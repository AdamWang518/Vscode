import requests
from bs4 import BeautifulSoup
import time
path = 'D:\\VScode\\Python\\ESJ-download\\贞操逆转世界的童贞边境领主骑士.txt'
f = open(path, 'w',encoding='utf8')
html=requests.get('https://www.esjzone.cc/detail/1652100515.html')
soup=BeautifulSoup(html.text,'html.parser')
Links=soup.select('#chapterList > a')
pageList=[]
for link in Links:
    pageList.append(link.get('href'))
#del pageList[0:10]
for page in pageList:
    html=requests.get(page)
    soup=BeautifulSoup(html.text,'html.parser')
    #time.sleep(1)
    title=soup.select('body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > h2')[0].string
    words=soup.select('body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > div.forum-content.mt-3 > p')
    f.write(title+'\n')
    for word in words:
        f.write(word.text+'\n')
print('結束')
    
