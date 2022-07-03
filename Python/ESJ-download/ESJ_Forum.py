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
driver.get("https://www.esjzone.cc/forum/1584679807/1611288898/")#要爬的小說連結
driver.find_element_by_xpath('//*[@id="ticrf"]/div/div/div[3]/a[2]').click()
driver.find_element_by_xpath('//*[@id="dataTable"]/thead/tr/th[2]/div[1]').click()