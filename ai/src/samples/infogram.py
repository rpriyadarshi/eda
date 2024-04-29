# https://stackoverflow.com/questions/63700789/how-to-extract-data-from-graph-from-a-web-page
import requests
from bs4 import BeautifulSoup
import re
import json

original_url = "https://cawp.rutgers.edu/women-percentage-2020-candidates"
r = requests.get(original_url)
soup = BeautifulSoup(r.text, "html.parser")

infogram_url = f'https://e.infogram.com/{soup.find("div",{"class":"infogram-embed"})["data-id"]}'
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

data = []
for t in tables:
    for i, sheet in enumerate(t[0]):
        data.append({
            "sheetName": sheet,
            "table": dict([(t[1][i][0][j],t[1][i][1][j])  for j in range(len(t[1][i][0])) ])
        })
print(data)
