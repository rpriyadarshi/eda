# https://python.langchain.com/docs/use_cases/web_scraping/
from langchain_openai import OpenAIEmbeddings
from langchain.text_splitter import RecursiveCharacterTextSplitter
from langchain_community.vectorstores import DocArrayInMemorySearch
from langchain_community.vectorstores import Chroma
from langchain.chains import ConversationalRetrievalChain
from langchain_openai.chat_models import ChatOpenAI
from langchain_community.document_loaders import AsyncChromiumLoader
from langchain_community.document_transformers import Html2TextTransformer
from langchain_community.document_transformers import BeautifulSoupTransformer

import param

class AICrawler(param.Parameterized):
    model_name = param.String(default="gpt-3.5-turbo", doc="Open AI model name")
    urls = param.List(doc="Urls to process")
    chain_type = param.String(default="stuff", doc="Chain type")
    k = param.Integer(default=4, doc="Retriever search kwargs")

    title = param.String(default="Ai Crawler", doc="Title of this application")
    persist_directory = param.String(default="docs/chroma/", doc="Chroma database persistance directory")
    delimiter = param.String(default="####", doc="User input delimiter")
    system = param.String(default="", doc="System prompt")

    chat_history = param.List([])
    answer = param.String("")
    db_query  = param.String("")
    db_response = param.List([])

    def __init__(self, **params):
        super(AICrawler, self).__init__( **params)

        # Get OpenAI model
        self.model = self.get_model()

        # # Prompt
        # self.prompt = self.get_prompt()

        # Get embeddings
        self.embeddings = self.get_embeddings()

        # Processed html page
        self.docs = self.load_url()

        # Built vector database
        self.db = self.get_chroma_db()

        # Setup the conversation reterival chain
        self.qa = self.get_conversation_reterival_chain()

    def get_model(self):
        return ChatOpenAI(model_name=self.model_name, temperature=0)

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

    def load_url(self):
        # load documents
        loader = AsyncChromiumLoader(urls=self.urls)
        htmldocs = loader.load()

        # html2text = Html2TextTransformer()
        # documents = html2text.transform_documents(htmldocs)
        bs_transformer = BeautifulSoupTransformer()
        documents = bs_transformer.transform_documents(htmldocs)

        # split documents
        text_splitter = RecursiveCharacterTextSplitter(chunk_size=1000, chunk_overlap=150)
        return text_splitter.split_documents(documents)

    def get_prompt(self, query):
        return {"system": self.system,
                "question": f"{self.delimiter}{query}{self.delimiter}", 
                "chat_history": self.chat_history}

    def ask(self, query):
        prompt = self.get_prompt(query)
        result = self.qa(prompt)

        self.answer = result['answer'] 
        self.chat_history.extend([(query, result["answer"])])
        self.db_query = result["generated_question"]
        self.db_response = result["source_documents"]
        return self.answer

    def clr_history(self,count=0):
        self.chat_history = []
        return 

delimiter = "####"
system = f"""
You are helpful but sassy assistant.
You will be provided with customer service queries.
The customer service query will be delimited with {delimiter} characters.
output a Python list of objects, where each object has the following format:
    'products': <a list of products that must be found in the allowed products below>

Where the categories and products must be found in the customer service query.
If a product is mentioned, it must be associated with the correct category in the allowed products list below.
If no products or categories are found, output an empty list.

You must process all images you encounter to extract embedded text.
You must process all videos to extract embedded text.
You must process all audio embedded in videos to convert to text.
You must process all audio files to convert to text.
You must cycle through all overlapping images you encounter to extract embedded text.

Allowed products:

Processor category:
    Intel Core Ultra 7 155H
    Apple M2
    Intel Core i7-1360P
    AMD Ryzen 7 7736U
    Intel Core i7-1255U
"""

# urls = ["https://www.pcmag.com/news/meteor-lake-first-tests-intel-core-ultra-7-benched-for-cpu-graphics-and"]
urls = ["https://pcmag.infogram.com/1pegzn0n7j6lgmtm9r7x95nwm9aln1qdq53"]

ac = AICrawler(urls=urls, model_name="gpt-3.5-turbo", system=system)

result = ac.ask("Please list all the processors you have found and CPU performance tests")
# print(ac.chat_history)
# print(ac.db_query)
# print(ac.db_response)
print(result)
