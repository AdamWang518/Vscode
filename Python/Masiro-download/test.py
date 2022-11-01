import undetected_chromedriver as uc
from time import sleep
import time
browser = uc.Chrome(use_subprocess=True)
browser.get('https://google.com')
time.sleep(100)
html = browser.page_source
