import sys
import pprint

from ai_crawler import AICrawler

delimiter = "####"
system0 = f"""
You are helpful but sassy assistant.
You will be provided with customer service queries.
The customer service query will be delimited with {delimiter} characters.

Processor and CPU are used interchangebly.
You must use the loaded document to answer queries.

product_name: is the name of product, like a laptop, examples are listed below
benchmark: an integer value of the benchmark value or score
benchmark_name: is the name of benchmark that were run to obtain benchmark score. Examples are listed below
allowed_processors: is a processor or CPU. The examples are listed below

Product examples:
    Acer Swift Go 14
    Apple MacBook Air 15-inch
    Framework 13 (2023)
    HP Dragonfly Pro
    Lenovo Yoga 9i Gen 8
    Microsoft Surface Laptop 5 (15-Inch)
    Qualcomm Snapdragon X Elite (Config A)
    Qualcomm Snapdragon X Elite (Config B)

Processor examples:
    Intel Core Ultra 7 155H
    Apple M2
    Intel Core i7-1360P
    AMD Ryzen 7 7736U
    Intel Core i7-1255U
    Qualcomm Snapdragon X Elite

Benchmark examples:
    Cinebench 2024
    Cinebench R23
    Geekbench 6.2
    3DMark
    GFXBench
    PCMark 10
"""

# system1 = f"""
# You are helpful but sassy assistant.
# You will be provided with customer service queries.
# The customer service query will be delimited with {delimiter} characters.
# output a Python list of objects, where each object has the following format:
#     'products': <a list of products that must be found in the allowed products below>
#     'processor': <a list of processors that must be found in the allowed processors below>
#     'benchmark': <a list of benchmarks that must be found in the allowed benchmarks below>

# Where the categories and products can be found in the customer service query.
# If a product is mentioned, it must be associated with the correct category in the allowed products list below.
# Any products found must also be associated with the correct category in the allowed products list below.
# If no products or categories are found, output an empty list.

# Processor and CPU are used interchangebly.

# You must process all images you encounter to extract embedded text.
# You must process all videos to extract embedded text.
# You must process all audio embedded in videos to convert to text.
# You must process all audio files to convert to text.
# You must cycle through all overlapping images you encounter to extract embedded text.

# Allowed products:
#     Acer Swift Go 14
#     Apple MacBook Air 15-inch
#     Framework 13 (2023)
#     HP Dragonfly Pro
#     Lenovo Yoga 9i Gen 8
#     Microsoft Surface Laptop 5 (15-Inch)
#     Qualcomm Snapdragon X Elite (Config A)
#     Qualcomm Snapdragon X Elite (Config B)

# Processor category:
#     Intel Core Ultra 7 155H
#     Apple M2
#     Intel Core i7-1360P
#     AMD Ryzen 7 7736U
#     Intel Core i7-1255U
#     Qualcomm Snapdragon X Elite

# Allowed benchmarks:
#     Cinebench 2024
#     Cinebench R23
#     Geekbench 6.2
# """

schema = {
    "properties": {
        "product_name": {"type": "string"},
        "benchmark": {"type": "integer"},
        "benchmark_name": {"type": "string"},
        "allowed_processors": {"type": "string"}
    },
    "required": ["product_name", "benchmark_name", "benchmark", "allowed_processors"],
}

#-------------------------------------------------------------------------------
# Build crawler with URL list
#-------------------------------------------------------------------------------
urls = ["https://www.pcmag.com/news/meteor-lake-first-tests-intel-core-ultra-7-benched-for-cpu-graphics-and"]
# urls = ["https://pcmag.infogram.com/1pegzn0n7j6lgmtm9r7x95nwm9aln1qdq53"]

#-------------------------------------------------------------------------------
# Inject debug htmldoc
#-------------------------------------------------------------------------------
# htmldocs_path = "/home/rohitpri/GitHub/eda/ai/src/cache/htmldocs.small.json"
# htmldocs_path = "/home/rohitpri/GitHub/eda/ai/src/cache/htmldocs.full.json"
# htmldocs_path = "/home/rohitpri/GitHub/eda/ai/src/cache/hacked.json"
htmldocs_path = ""

#-------------------------------------------------------------------------------
# Run queries
#-------------------------------------------------------------------------------
def query_help():
    print("""
Options:
    exit, quit:   Exit the ai loop
    extract:      Run the JSON extractor with supplied schema
    htmldocs:     Debug dump of extracted html
    documents:    Debug dump of extracted text from html
    docs:         Split documents that will be pasted in ai context window
    help:         This message
    """)

def run_queries(ac: AICrawler):
    query = ""
    while True:
        query = input('crawler>: ')
        if query in ["exit", "quit"]:
            break
        elif query in ["extract"]:
            extracted_content = ac.extract()
            ac.write_json_str("output/extracted_data.json", extracted_content)
        elif query in ["htmldocs"]:
            pprint.pprint(ac.htmldocs)
        elif query in ["documents"]:
            pprint.pprint(ac.documents)
        elif query in ["docs"]:
            pprint.pprint(ac.docs)
        elif query in ["help"]:
            query_help()
        else:
            result = ac.ask(query)
            # print(ac.chat_history)
            # print(ac.db_query)
            # print(ac.db_response)
            print(result)

#-------------------------------------------------------------------------------
# Run flow
#-------------------------------------------------------------------------------
def flow_help():
    print("""
Options:
    exit, quit:   Exit the ai loop
    crawler:      Run AI crawler
    help:         This message
    """)

def run_flow():
    query = ""
    while True:
        query = input('flow>: ')
        if query in ["exit", "quit"]:
            break
        elif query in ["crawler"]:
            ac = AICrawler(urls=urls, 
                        model_name="gpt-3.5-turbo", 
                        delimiter=delimiter,
                        system=system0, 
                        schema=schema, 
                        htmldocs_path=htmldocs_path)
            run_queries(ac)
        elif query in ["help"]:
            flow_help()

#-------------------------------------------------------------------------------
# Main et.el.
#-------------------------------------------------------------------------------
def main():
    run_flow()

if __name__ == "__main__":
    main()
