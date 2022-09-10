from ast import Delete
from time import sleep
from selenium import webdriver
from bs4 import BeautifulSoup
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
import time
driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()))
driver.get("https://masiro.me/admin/auth/login")
driver.find_element(By.XPATH,'//*[@id="username"]').send_keys('dodoga518@gmail.com')
driver.find_element(By.XPATH,'//*[@id="password"]').send_keys('20010518')
driver.find_element(By.XPATH,'//*[@id="login-btn"]').click()
sleep(5)
#driver.find_element_by_xpath('/html/body/div/header/nav/a').click()
driver.get("https://masiro.me/admin/novelView?novel_id=243")#輸入需要的小說連結
soup = BeautifulSoup(driver.page_source, 'html.parser')
try:
    name=soup.select('#app > section.content > div:nth-child(1) > div > div > div.box-body.z-i > div.novel-title')[0].string
    path = 'D:\\MEGA\\真白萌爬蟲\\'+name+'.txt'
    f = open(path, 'w',encoding='utf8')
    Links=soup.select('#app > section.content > div > div > div > div.box-body > div.chapter-content > ul > li > ul > a')    
    pageList=[]
    for link in Links:
        pageList.append('https://masiro.me'+link.get('href'))
    for page in pageList:
        driver.get(page)
        soup=BeautifulSoup(driver.page_source,'html.parser')
        #time.sleep(1)
        time.sleep(1)
        title=soup.select('#app > section.content > div:nth-child(1) > div > div > div.box-header.with-border.nov-title-box > span.novel-title > div')[0].string
        words=soup.select('#app > section.content > div:nth-child(1) > div > div > div.box-body.nvl-content >p')
        # brs=soup.select('#app > section.content > div:nth-child(1) > div > div > div.box-body.nvl-content>br')
        #app > section.content > div:nth-child(1) > div > div > div.box-body.nvl-content > br:nth-child(63)
        # f.write(title+'\n')
        for word in words:
            f.write(word.text+'\n')

        # for br in brs:
        #     f.write(br.text+'\n')
    f.close()
except:
    print('有問題')
print('結束')


