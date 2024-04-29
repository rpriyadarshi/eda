from langchain_community.document_loaders import AsyncChromiumLoader
from openai import OpenAI
import os

openai_api_key=os.environ['OPENAI_API_KEY']
client = OpenAI(api_key=openai_api_key)

# Set up your OpenAI API key

# Define the HTML content of the web page
html_content = """
<html>
<head>
<title>Example Web Page</title>
</head>
<body>
<h1>Welcome to Example Web Page</h1>
<p>This is some sample text from the web page.</p>
</body>
</html>
"""
#-------------------------------------------------------------------------------
# Build crawler with URL list
#-------------------------------------------------------------------------------
urls = ["https://www.pcmag.com/news/meteor-lake-first-tests-intel-core-ultra-7-benched-for-cpu-graphics-and"]
# urls = ["https://pcmag.infogram.com/1pegzn0n7j6lgmtm9r7x95nwm9aln1qdq53"]

loader = AsyncChromiumLoader(urls=urls)
htmldocs = loader.load()
html_content = htmldocs[0].page_content

# Define the prompt for the model
prompt = "Extract information from the following web page:\n" + html_content

# Generate response from the model
response = client.completions.create(
    model="gpt-3.5-turbo-instruct",  # Use a suitable engine for text processing
    prompt=prompt,
    max_tokens=100
)

# Extracted information from the model's response
extracted_info = response.choices[0].text.strip()

print("Extracted Information:")
print(extracted_info)
