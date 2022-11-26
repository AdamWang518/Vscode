from bs4 import BeautifulSoup
import requests
import lxml
from firebase import firebase
firebaseURL="https://newsdb-6da43-default-rtdb.firebaseio.com/"
fb=firebase.FirebaseApplication(firebaseURL,None)
res = requests.get("https://news.ltn.com.tw/rss/business.xml") 
soup = BeautifulSoup(res.text,'xml') 
links = soup.find_all('link')
del links[0]
for link in links:
    data=dict()
    res = requests.get(link.text)
    soup = BeautifulSoup(res.text,'html.parser') 
    title=soup.select('div.whitecon.boxTitle.boxText > h1')[0].text
    # 獲取標題
    context=str(soup)
    #獲取內文html
    date=soup.select('div.whitecon.boxTitle.boxText > div.function > span')[0].text
    #獲取日期
    category="財經"
    #獲取類別
    data["title"]=title
    data["context"]=context
    data["date"]=date
    data["category"]=category
    fb.post('/news',data)


