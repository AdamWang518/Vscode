from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
gallery="https://www.ss.net.tw/index.php?view=artist"
driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()))
driver.get(gallery)
for i in range(1,28):
    style=str(driver.find_element(By.CSS_SELECTOR,"#A"+str(i)+" > div > a:nth-child(1)").get_attribute("href")).replace("#title","")
    print(style)
