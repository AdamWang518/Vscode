from ast import Delete
from time import sleep
from selenium import webdriver
from bs4 import BeautifulSoup
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager

from selenium.webdriver.chrome.options import Options
import time



driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()))


driver.get("https://masiro.me/admin/auth/login")

driver.find_element(By.XPATH,'//*[@id="username"]').send_keys('dodoga518@gmail.com')
driver.find_element(By.XPATH,'//*[@id="password"]').send_keys('20010518')
driver.find_element(By.XPATH,'//*[@id="login-btn"]').click()

time.sleep(5)
with open(f'D:\\Vscode\\Python\\Masiro-download\\Masiro_List.txt','r',encoding='utf8') as listReader:
    for line in listReader:
        driver.get(line)#要爬的小說連結
        try:
            driver.find_element(By.XPATH,'//*[@id="app"]/section[2]/div[1]/button').click()
        except:
            print("",end="")
        

        name=driver.find_element(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-body.z-i > div.novel-title').text
        print(name)
        print(line)
        path = 'D:\\MEGA\\小說\\真白萌爬蟲\\'+name+'.txt'
        f = open(path, 'w',encoding='utf8')
        f.write('小說連結:'+line+'\n\n')
        Links=driver.find_elements(By.CSS_SELECTOR,'#app > section.content > div > div > div > div.box-body > div.chapter-content > ul > li > ul > a')
        pageList=[]
        for link in Links:
            pageList.append(link.get_attribute('href'))
        for page in pageList:
            driver.get(page)
            try:
                #app > section.content > div:nth-child(1) > div > div > div.box-header.with-border.nov-title-box > span.novel-title > div
                title=driver.find_element(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-header.with-border.nov-title-box > span.novel-title > div').text
                words=driver.find_elements(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-body.nvl-content')
            except:
                print('付錢')
                driver.find_element(By.XPATH,'/html/body/div[1]/div[2]/div/section[2]/div/div/div/p').click()
                time.sleep(2)
                driver.find_element(By.XPATH,'/html/body/div[3]/div[3]/a[1]').click()
                time.sleep(2)
                driver.get(page)
                time.sleep(2)
                title=driver.find_element(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-header.with-border.nov-title-box > span.novel-title > div').text
                words=driver.find_elements(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-body.nvl-content')
            f.write(title+'\n')
            for word in words:
                    f.write(word.get_attribute('innerText'))
            f.write('\n')
f.close()
print('結束')