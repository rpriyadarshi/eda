from langchain_openai import OpenAIEmbeddings
from langchain.text_splitter import RecursiveCharacterTextSplitter
from langchain_community.vectorstores import DocArrayInMemorySearch
from langchain_community.vectorstores import Chroma
from langchain.chains import ConversationalRetrievalChain
from langchain_openai.chat_models import ChatOpenAI
from langchain_community.document_loaders import AsyncChromiumLoader
from langchain_community.document_transformers import Html2TextTransformer

import param

class AICrawler(param.Parameterized):
    model_name = param.String(default="gpt-3.5-turbo", doc="Open AI model name")
    urls = param.List(doc="Urls to process")
    chain_type = param.String(default="stuff", doc="Chain type")
    k = param.Integer(default=4, doc="Retriever search kwargs")

    chat_history = param.List([])
    answer = param.String("")
    db_query  = param.String("")
    db_response = param.List([])

    def __init__(self, **params):
        super(AICrawler, self).__init__( **params)

        self.title = 'Ai Crawler'
        
        # Get OpenAI model
        self.model = self.get_model()

        # Get embeddings
        self.embeddings = self.get_embeddings()

        # Processed html page
        self.docs = self.load_url()

        # Built vector database
        self.db = self.get_chroma_db()

        # Setup the conversation reterival chain
        self.convchain = self.get_conversation_reterival_chain()

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
            embedding=self.embeddings
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
        html2text = Html2TextTransformer()
        documents = html2text.transform_documents(htmldocs)

        # split documents
        text_splitter = RecursiveCharacterTextSplitter(chunk_size=1000, chunk_overlap=150)
        return text_splitter.split_documents(documents)
    
    def ask(self, query):
        result = self.convchain({"question": query, "chat_history": self.chat_history})

        self.answer = result['answer'] 
        self.chat_history.extend([(query, result["answer"])])
        self.db_query = result["generated_question"]
        self.db_response = result["source_documents"]
        return self.answer

    def clr_history(self,count=0):
        self.chat_history = []
        return 

# urls = ["https://www.pcmag.com/reviews/intel-core-i9-14900ks"]

# ac = AICrawler(urls=urls, model_name="gpt-3.5-turbo")

# result = ac.ask("Please summarize information on CPU")
# print(ac.chat_history)
# print(ac.db_query)
# print(ac.db_response)
# print(result)
