from time import sleep
from selenium import webdriver
from bs4 import BeautifulSoup
import time
driver = webdriver.Chrome("D:\\Vscode\\Python\\ESJ-download\\chromedriver.exe")
driver.get("https://www.esjzone.cc/my/login")
driver.find_element_by_xpath('/html/body/div[3]/section/div/div[1]/form/div[1]/input').send_keys('dodoga518@gmail.com')
driver.find_element_by_xpath('/html/body/div[3]/section/div/div[1]/form/div[2]/input').send_keys('20010518')
driver.find_element_by_xpath('/html/body/div[3]/section/div/div[1]/form/div[4]/a').click()
time.sleep(5)
driver.get("https://www.esjzone.cc/detail/1592280550.html")#要爬的小說連結
soup = BeautifulSoup(driver.page_source, 'html.parser')
name=soup.select('body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > div:nth-child(1) > div.col-md-9.book-detail > h2')[0].string
path = 'D:\\VScode\\Python\\ESJ-download\\'+name+'.txt'#要爬的小說名稱
f = open(path, 'w',encoding='utf8')
Links=soup.select('#chapterList > a')
pageList=[]
for link in Links:
    pageList.append(link.get('href'))
for page in pageList:
    driver.get(page)
    soup=BeautifulSoup(driver.page_source,'html.parser')
    #time.sleep(1)
    title=soup.select('body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > h2')[0].string
    words=soup.select('body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > div.forum-content.mt-3 > p')
    f.write(title+'\n')
    for word in words:
        f.write(word.text+'\n')
print('結束')