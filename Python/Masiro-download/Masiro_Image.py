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
time.sleep(5)

driver.get("https://masiro.me/admin/novelView?novel_id=779")#要爬的小說連結


name=driver.find_element(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-body.z-i > div.novel-title').text
# path = 'D:\\MEGA\\小說\\真白萌爬蟲\\'+'『男女比1比30』世界的黑一點偶像'+'.txt'#要爬的小說名稱
path = 'D:\\MEGA\\小說\\真白萌爬蟲\\'+name+'.txt'
# f = open(path, 'w',encoding='utf8')
# Details=soup.select('#chapterList > details > a')
Links=driver.find_elements(By.CSS_SELECTOR,'#app > section.content > div > div > div > div.box-body > div.chapter-content > ul > li > ul > a')
pageList=[]
# for detail in Details:
#     pageList.append(detail.get('href'))
for link in Links:
    pageList.append(link.get_attribute('href'))
    # print(link.get_attribute('href'))
# del pageList[0:43]
for page in pageList:
    driver.get(page)
    time.sleep(1)
    title=driver.find_element(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-header.with-border.nov-title-box > span.novel-title > div').text
    words=driver.find_elements(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-body.nvl-content')

    soup = BeautifulSoup(driver.page_source, 'html.parser')
    print(soup.select('#app > section.content > div:nth-child(1) > div > div > div.box-body.nvl-content')[0])
    imageSRC=driver.find_elements(By.CSS_SELECTOR,'.nvl-content img')
    imageList=[]
    for image in imageSRC:
        imageList.append(image.get_attribute('src'))
    print(imageList)
    # f.write(title+'\n')
    # for word in words:
    #     f.write(word.text)
    # f.write('\n')
# f.close()
print('結束')