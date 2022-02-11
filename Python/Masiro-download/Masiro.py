from selenium import webdriver
path = 'D:\\VScode\\Python\\Masiro-download\\bookName.txt'
f = open(path, 'w',encoding='utf8')
driver = webdriver.Chrome("D:\\Vscode\\Python\\Masiro-download\\chromedriver.exe")
driver.get("https://masiro.me/admin/auth/login")
driver.find_element_by_xpath('//*[@id="username"]').send_keys('dodoga518@gmail.com')
driver.find_element_by_xpath('//*[@id="password"]').send_keys('20010518')
driver.find_element_by_xpath('//*[@id="login-btn"]').click()
driver.find_element_by_xpath('/html/body/div/header/nav/a').click()
