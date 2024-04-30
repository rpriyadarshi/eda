# https://stackoverflow.com/questions/63700789/how-to-extract-data-from-graph-from-a-web-page
import requests
from bs4 import BeautifulSoup
import re
import json
import pprint
import param

class InfogramLoader(param.Parameterized):
    def __init__(self, **params):
        super(InfogramLoader, self).__init__( **params)

        self.data = {}

    def extract_tables(self, infogram):
        infogram_url = f'https://e.infogram.com/{infogram["data-id"]}'
        r = requests.get(infogram_url)
        soup = BeautifulSoup(r.text, "html.parser")

        script = [
            t 
            for t in soup.findAll("script") 
            if "window.infographicData" in t.text
        ][0].text

        extract = re.search(r".*window\.infographicData=(.*);$", script)
        data = json.loads(extract.group(1))
        entities = data["elements"]["content"]["content"]["entities"]

        tables = [
            (entities[key]["props"]["chartData"]["sheetnames"], entities[key]["props"]["chartData"]["data"])
            for key in entities.keys()
            if ("props" in entities[key]) and ("chartData" in entities[key]["props"])
        ]
        return infogram_url, tables

    def extract_data(self, infogram_url, tables):
        infogram_data = {}
        for t in tables:
            for i, sheet in enumerate(t[0]):
                cols = []
                i_len = len(t[1][i])
                for j in range(i_len):
                    j_len = len(t[1][i][j])
                    rows = []
                    for k in range(j_len):
                        var = t[1][i][j][k]
                        if type(var) == type({}):
                            value = var["value"]
                        else:
                            value = var
                        rows.append(value)
                    cols.append(rows)
                infogram_data[sheet] = cols
        self.data[infogram_url] = infogram_data

    def load(self, urls):
        for url in urls:
            r = requests.get(url)
            soup = BeautifulSoup(r.text, "html.parser")

            infograms = soup.find_all("div",{"class":"infogram-embed"})
            for infogram in infograms:
                infogram_url, tables = self.extract_tables(infogram)
                self.extract_data(infogram_url, tables)

# urls = ["https://cawp.rutgers.edu/women-percentage-2020-candidates",
#         "https://www.pcmag.com/news/meteor-lake-first-tests-intel-core-ultra-7-benched-for-cpu-graphics-and"]
# ie = InfogramLoader()
# ie.load(urls)
# print(ie.data)
