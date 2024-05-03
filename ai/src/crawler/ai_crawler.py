# https://python.langchain.com/docs/use_cases/web_scraping/
# https://python.langchain.com/docs/modules/model_io/chat/structured_output/

import requests
import os
import param
import pprint
import json
import asyncio

from bs4 import BeautifulSoup
from typing import List

from langchain_openai import OpenAIEmbeddings
from langchain.text_splitter import Language
from langchain.text_splitter import RecursiveCharacterTextSplitter
from langchain_community.vectorstores import DocArrayInMemorySearch
from langchain_community.vectorstores import Chroma
from langchain.chains import ConversationalRetrievalChain
from langchain_openai.chat_models import ChatOpenAI
from langchain_community.document_loaders import AsyncChromiumLoader
from langchain_community.document_transformers import BeautifulSoupTransformer
from langchain_community.document_transformers import Html2TextTransformer
from langchain.chains import create_extraction_chain
from langchain_community.docstore.document import Document
from langchain_community.document_loaders import JSONLoader
from langchain_community.document_loaders.recursive_url_loader import RecursiveUrlLoader
from langchain_community.document_loaders.merge import MergedDataLoader
from langchain_core.pydantic_v1 import BaseModel

from langchain_community.document_loaders import BSHTMLLoader
from infogram_loader import InfogramLoader

class BenchmarkData(BaseModel):
        """JSON schema of the expected output. 
        product_name: is the name of product, like a laptop.
        benchmark: an integer value of the benchmark value or score.
        benchmark_name: is the name of benchmark that were run to obtain benchmark score.
        allowed_processors: is a processor or CPU.
        """
        product_name: str
        benchmark: int
        benchmark_name: str
        allowed_processors: list

class StructuredData(BaseModel):
        """JSON schema of the expected output. 
        benchmark_data: is the list of all the benchmark data found.
        """
        benchmark_data: list

class AICrawler(param.Parameterized):
    model_name = param.String(default="gpt-3.5-turbo", doc="Open AI model name")
    urls = param.List(doc="Urls to process")
    chain_type = param.String(default="stuff", doc="Chain type")
    k = param.Integer(default=4, doc="Retriever search kwargs")

    title = param.String(default="Ai Crawler", doc="Title of this application")
    persist_directory = param.String(default="docs/chroma/", doc="Chroma database persistance directory")
    delimiter = param.String(default="####", doc="User input delimiter")
    system = param.String(default="", doc="System prompt")
    schema = param.Dict(default={}, doc="Schema for extraction")
    htmldocs_path = param.String(default="", doc="Load html docs from this file")

    chat_history = param.List([])
    answer = param.String("")
    db_query  = param.String("")
    db_response = param.List([])

    def __init__(self, **params):
        super(AICrawler, self).__init__( **params)

        # Get OpenAI model
        self.model = self.get_model()

        self.structured_llm = self.get_structured_llm()

        # Get embeddings
        self.embeddings = self.get_embeddings()

        # Processed html page
        self.docs = self.load_url()

        # Built vector database
        # self.db = self.get_chroma_db()
        self.db = self.get_in_memory_db()

        # Setup the conversation reterival chain
        self.qa = self.get_conversation_reterival_chain()

    def get_model(self):
        return ChatOpenAI(model_name=self.model_name, temperature=0)

    def get_structured_llm(self):
        return self.model.with_structured_output(
            # StructuredData,
            method = "json_mode",
            include_raw=True
        )

    def get_embeddings(self):
        return OpenAIEmbeddings()

    def get_in_memory_db(self):
        return DocArrayInMemorySearch.from_documents(
            documents=self.docs,
            embedding=self.embeddings
        )

    def get_chroma_db(self):
        return Chroma.from_documents(
            documents=self.docs,
            embedding=self.embeddings,
            persist_directory=self.persist_directory
        )
    
    def get_retriever(self):
        return self.db.as_retriever(search_type="similarity", search_kwargs={"k": self.k})

    def get_conversation_reterival_chain(self):
        return ConversationalRetrievalChain.from_llm(
            llm=self.model, 
            chain_type=self.chain_type, 
            retriever=self.get_retriever(), 
            return_source_documents=True,
            return_generated_question=True
        )

    def load_infograms(self):
        self.infogram_loader = InfogramLoader(urls=self.urls)
        self.infogramdocs = self.infogram_loader.extract()

    def load_htmldocs(self):
        # load documents
        if not self.htmldocs_path:
            loader_web = AsyncChromiumLoader(urls=self.urls)
            loader_infogram = BSHTMLLoader(self.infogram_loader.infogram_cache)
            loader = MergedDataLoader(loaders=[loader_web, loader_infogram])
            # loader = RecursiveUrlLoader(
            #     url=self.urls[0], max_depth=10
            # )
            self.htmldocs = loader.load()

            self.write_json("output/htmldocs.json", self.htmldocs)

            # html = asyncio.run(loader.ascrape_playwright(url=self.urls[0]))
            # self.write_json_str("output/htmldocs_str.json", html)
        else:
            loader = JSONLoader(
                file_path=self.htmldocs_path,
                jq_schema=".[].page_content",
                text_content=False
            )
            self.htmldocs = loader.load()

    def transform_htmldocs(self):
        transformer = Html2TextTransformer()
        # transformer = BeautifulSoupTransformer()
        self.documents = transformer.transform_documents(self.htmldocs)

    def split_document(self):
        # split documents
        text_splitter = RecursiveCharacterTextSplitter(chunk_size=4000, chunk_overlap=150)
        return text_splitter.split_documents(self.documents)

    def split_htmldoc(self):
        # split documents
        html_splitter = RecursiveCharacterTextSplitter.from_language(language=Language.HTML, chunk_size=500, chunk_overlap=50)
        return html_splitter.create_documents(self.htmldocs[0].page_content)

    def load_url(self):
        self.load_infograms()
        self.load_htmldocs()
        self.transform_htmldocs()
        return self.split_document()
        # return self.split_htmldoc()

    def extract(self):
        content = []
        for doc in self.docs:
            extracted_content = create_extraction_chain(schema=self.schema, llm=self.model).run(doc)
            for ec in extracted_content:
                content.append(ec)
        return content

    def get_prompt(self, query):
        return {"system": self.system,
                "question": f"{self.delimiter}{query}{self.delimiter}", 
                "chat_history": self.chat_history}

    def get_prompt_json(self, query):
        # return f"""Here is the pretext: {self.system} 
        return f"""Answer the following question. 
        Make sure to return a JSON blob with keys shown here:
        'product_name': is the name of product, like a laptop.
        'benchmark': an integer value of the benchmark value or score.
        'benchmark_name': is the name of benchmark that were run to obtain benchmark score.
        'allowed_processors': is a processor or CPU.
        {self.delimiter}{query}{self.delimiter}"""

    def ask(self, query):
        prompt = self.get_prompt(query)
        result = self.qa(prompt)

        self.answer = result['answer'] 
        self.chat_history.extend([(query, result["answer"])])
        self.db_query = result["generated_question"]
        self.db_response = result["source_documents"]
        return self.answer

    def ask_structured(self, query):
        prompt = self.get_prompt_json(query)
        result = self.structured_llm.invoke(prompt)

        # self.answer = result['parsed'] 
        # self.chat_history.extend([(query, result["parsed"])])
        # self.db_query = result["generated_question"]
        # self.db_response = result["source_documents"]
        pprint.pprint(result)
        self.answer = "*** WIP ***"
        return self.answer

    def clr_history(self, count=0):
        self.chat_history = []
        return
    
    def write_json(self, path: str, htmldocs) -> None:
        results = [{"page_content": doc.page_content, "metadata": doc.metadata} for doc in htmldocs]
        with open(path, "w") as f:
            json.dump(results, f, indent=2)

    def write_json_str(self, path: str, htmldocs) -> None:
        with open(path, "w") as f:
            json.dump(htmldocs, f, indent=2)
