from ast import Delete
from time import sleep
from selenium import webdriver
from bs4 import BeautifulSoup
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
import time

driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()))
# driver = webdriver.Chrome("D:\\Vscode\\Python\\ESJ-download\\chromedriver.exe")
driver.get("https://www.esjzone.cc/my/login")
driver.find_element(By.XPATH,'/html/body/div[3]/section/div/div[1]/form/div[1]/input').send_keys('dodoga518@gmail.com')
driver.find_element(By.XPATH,'/html/body/div[3]/section/div/div[1]/form/div[2]/input').send_keys('20010518')
driver.find_element(By.XPATH,'/html/body/div[3]/section/div/div[1]/form/div[4]/a').click()
time.sleep(5)
driver.get("https://www.esjzone.cc/detail/1617754811.html")#要爬的小說連結
# driver.find_element(By.XPATH,'//*[@id="chapterList"]/details[1]/summary/strong').click()
# driver.find_element(By.XPATH,'//*[@id="chapterList"]/details[2]/summary/strong').click()

name=driver.find_element(By.CSS_SELECTOR,'body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > div:nth-child(1) > div.col-md-9.book-detail > h2').text
# path = 'D:\\MEGA\\ESJ爬蟲\\'+'『男女比1比30』世界的黑一點偶像'+'.txt'#要爬的小說名稱
path = 'D:\\MEGA\\ESJ爬蟲\\'+name+'.txt'
f = open(path, 'w',encoding='utf8')
# Details=soup.select('#chapterList > details > a')
Links=driver.find_elements(By.CSS_SELECTOR,'#chapterList > a')
pageList=[]
# for detail in Details:
#     pageList.append(detail.get('href'))
for link in Links:
    pageList.append(link.get_attribute('href'))
# del pageList[0]
for page in pageList:
    driver.get(page)
    time.sleep(1)
    title=driver.find_element(By.CSS_SELECTOR,'body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > h2').text
    words=driver.find_elements(By.CSS_SELECTOR,'body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > div.forum-content.mt-3')
    
    f.write(title+'\n')
    for word in words:
        f.write(word.text)
    f.write('\n')
f.close()
print('結束')