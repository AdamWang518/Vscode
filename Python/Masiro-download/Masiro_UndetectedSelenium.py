from ast import Delete
from time import sleep
from selenium import webdriver

from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
import undetected_chromedriver as uc

from selenium.webdriver.chrome.options import Options
import time



driver = uc.Chrome(use_subprocess=True)


driver.get("https://masiro.me/admin/auth/login")
time.sleep(10)
driver.find_element(By.XPATH,'//*[@id="username"]').send_keys('dodoga518@gmail.com')
driver.find_element(By.XPATH,'//*[@id="password"]').send_keys('20010518')
driver.find_element(By.XPATH,'//*[@id="login-btn"]').click()
time.sleep(5)

driver.get("https://masiro.me/admin/novelView?novel_id=902")#要爬的小說連結


name=driver.find_element(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-body.z-i > div.novel-title').text
# path = 'D:\\MEGA\\小說\\真白萌爬蟲\\'+'『男女比1比30』世界的黑一點偶像'+'.txt'#要爬的小說名稱
path = 'D:\\MEGA\\小說\\真白萌爬蟲\\'+name+'.txt'
f = open(path, 'w',encoding='utf8')
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
    try:
        #app > section.content > div:nth-child(1) > div > div > div.box-header.with-border.nov-title-box > span.novel-title > div
        title=driver.find_element(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-header.with-border.nov-title-box > span.novel-title > div').text
        words=driver.find_elements(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-body.nvl-content')
    except:
        print('付錢')
        time.sleep(5)
        driver.find_element(By.XPATH,'/html/body/div[1]/div[2]/div/section[2]/div/div/div/p').click()
        time.sleep(2)
        driver.find_element(By.XPATH,'/html/body/div[3]/div[3]/a[1]').click()
        time.sleep(2)
        driver.get(page)
        time.sleep(5)
        title=driver.find_element(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-header.with-border.nov-title-box > span.novel-title > div').text
        words=driver.find_elements(By.CSS_SELECTOR,'#app > section.content > div:nth-child(1) > div > div > div.box-body.nvl-content')
    f.write(title+'\n')
    for word in words:
        f.write(word.text)
    f.write('\n')
f.close()
driver.quit()
print('結束')
