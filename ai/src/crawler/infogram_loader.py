# https://stackoverflow.com/questions/63700789/how-to-extract-data-from-graph-from-a-web-page
# https://www.freeformatter.com/html-validator.html
import requests
from bs4 import BeautifulSoup
import re
import json
import pprint
import param
import pandas as pd


class InfogramLoader(param.Parameterized):
    urls = param.List(doc="Urls to process")

    def __init__(self, **params):
        super(InfogramLoader, self).__init__( **params)

        self.data = {}
        self.tab = 0
        self.tab_size = 2

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

# <!DOCTYPE html>
# <head>
#   <meta charset="UTF-8">
#   <title>Title of the document</title>
# </head>
# <table>
#   <caption>Concerts</caption>
#   <tr>
#     <th>Company</th>
#     <th>Contact</th>
#     <th>Country</th>
#   </tr>
#   <tr>
#     <td>Alfreds Futterkiste</td>
#     <td>Maria Anders</td>
#     <td>Germany</td>
#   </tr>
#   <tr>
#     <td>Centro comercial Moctezuma</td>
#     <td>Francisco Chang</td>
#     <td>Mexico</td>
#   </tr>
# </table>

    def get_spaces(self):
        return ' ' * self.tab
    
    def get_sep(self):
        return '\n' + self.get_spaces()
    
    def get_tab(self):
        return self.get_sep()

    def get_tabup(self):
        self.tab += self.tab_size
        return self.get_sep()
    
    def get_tabdn(self):
        self.tab -= self.tab_size
        return self.get_sep()
    
    def reset_tab(self):
        self.tab = 0

    def set_tab_size(self, size: int):
        self.tab_size = size

    def to_html(self):
        self.set_tab_size(2)
        html = f'<!DOCTYPE html>'
        html = self.get_tab().join([html, f'<head>'])
        html = self.get_tabup().join([html, f'<meta charset="UTF-8">'])
        html = self.get_tab().join([html, f'<title>Infogram Extraction</title>'])
        self.get_tabdn()
        html = self.get_tabdn().join([html, f'</head>'])

        for key0, value0 in self.data.items():
            html = self.get_tabup().join([html, f'<table>'])
            html = self.get_tabup().join([html, f'<caption>{key0}</caption>'])
            self.get_tabdn()
            
            html = self.get_tabup().join([html, f'<tr>'])
            html = self.get_tabup().join([html, f'<td>'])
            for key1, value1 in value0.items():
                html = self.get_tabup().join([html, f'<table>'])
                html = self.get_tabup().join([html, f'<caption>{key1}</caption>'])
                self.get_tabdn()

                html = self.get_tabup().join([html, f'<thead>'])
                html = self.get_tabup().join([html, f'<tr>'])

                self.get_tabup()
                for col in value1[0]:
                    html = self.get_tab().join([html, f'<th>{col}</th>'])
                html = self.get_tabdn().join([html, f'</tr>'])
                html = self.get_tabdn().join([html, f'</thead>'])
                self.get_tabdn()

                for row in value1[1:]:
                    html = self.get_tabup().join([html, f'<tr>'])
                    self.get_tabup()
                    for col in row:
                        html = self.get_tab().join([html, f'<td>{col}</td>'])
                    html = self.get_tabdn().join([html, f'</tr>'])
                    self.get_tabdn()

                html = self.get_tabdn().join([html, f'</table>'])
            html = self.get_tabdn().join([html, f'</td>'])
            html = self.get_tabdn().join([html, f'</tr>'])
            html = self.get_tabdn().join([html, f'</table>'])
        return html

    def load(self):
        for url in self.urls:
            r = requests.get(url)
            soup = BeautifulSoup(r.text, "html.parser")

            infograms = soup.find_all("div",{"class":"infogram-embed"})
            for infogram in infograms:
                infogram_url, tables = self.extract_tables(infogram)
                self.extract_data(infogram_url, tables)
        return self.to_html()

# urls = ["https://cawp.rutgers.edu/women-percentage-2020-candidates",
#         "https://www.pcmag.com/news/meteor-lake-first-tests-intel-core-ultra-7-benched-for-cpu-graphics-and"]
# ie = InfogramLoader(urls=urls)
# html = ie.load()
# print(html)
