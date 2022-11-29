
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
pageList = []
styles = []
drawArray = []
f = open("D:\\Vscode\\Python\\SQLHW\\painting\\links.text", "w", encoding='utf8')
gallery = "https://www.ss.net.tw/index.php?view=artist"
driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()))
driver.get(gallery)
for i in range(1, 28):
    styles.append(str(driver.find_element(By.CSS_SELECTOR, "#A"+str(i) +
                  " > div > a:nth-child(1)").get_attribute("href")).replace("#title", ""))
# 要爬的畫風
for style in styles:
    driver.get(style)
    lastPage = int(str(driver.find_elements(
        By.CLASS_NAME, 'page-link')[-1].get_attribute('href')).split('~')[-1])
    for i in range(1, lastPage+1):
        pagelink = style+"~"+str(i)
        driver.get(pagelink)
        # pageDict=dict()
        drawlinks = driver.find_elements(
            By.CSS_SELECTOR, 'div.row.row-cols-2.row-cols-sm-3.row-cols-md-4.row-cols-lg-5.row-cols-xl-6.align-items-end > div > div > a')
        for link in drawlinks:
            pageList.append(link.get_attribute('href'))
            f.write(link.get_attribute('href')+"\n")
f.close()
print('結束')



