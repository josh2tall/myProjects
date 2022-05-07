import sqlite3
from selenium.webdriver import Safari
from selenium.webdriver.safari.options import Options
from selenium.webdriver.common.by import By
import time

DB_FILE = "pythonsqlite.db"
BANDCAMP_FRONTPAGE='https://www.americanexpress.com/en-us/maps/online?cat=Shopping,Shop-Small'
TOTAL_PAGES = 20
TABLE_NAME = 'merchantInfo'

def insertMerchantInfo(merchantName, merchantWebsite, database: sqlite3.Connection):
    insertCommand = 'INSERT INTO "' + TABLE_NAME + '" VALUES("'+ merchantName + '","' + merchantWebsite + '")'
    database.execute(insertCommand)
    database.commit()

def getMerchantInfoExists(merchantName, database: sqlite3.Connection):
    getCommand = 'SELECT EXISTS(SELECT 1 FROM ' + TABLE_NAME + ' WHERE merchantName="'+ merchantName +'" LIMIT 1)'
    tempVar = database.execute(getCommand)
    #extracting value from tuple
    (returnValue,) = tempVar.fetchone()
    return returnValue

opts = Options()
opts.add_argument("--headless")
browser = Safari(options=opts)
browser.get(BANDCAMP_FRONTPAGE)
myDatabase = sqlite3.connect(DB_FILE)


#Use only once
# createTable = 'CREATE TABLE ' + TABLE_NAME + ' (merchantName text, merchantWebsite text)'
# myDatabase.execute(createTable)

for currentPage in range(TOTAL_PAGES):
    time.sleep(3)

    element = browser.find_element(By.XPATH, "/html/body")
    listItems = element.find_element(By.XPATH, '//*[@data-online-list="onlineListItem"]')
    merchantNames = listItems.find_elements(By.XPATH, '//*[@data-online-list="merchantName"]')
    merchantWebsites = listItems.find_elements(By.XPATH, '//*[@data-online-list="merchantWebsite"]')
    
    
    for x in range(len(merchantNames)):
        #0 is the merchant information doesn't exist in database
        if(getMerchantInfoExists(merchantNames[x].text, myDatabase) == 0):
            print('entered here')
            insertMerchantInfo(merchantNames[x].text, merchantWebsites[x].text, myDatabase)

    element.find_element(By.XPATH, '//*[@id="root"]/div/div/div/div/div/div/div/div/div[5]/div/div[2]/div/div[2]/div/div/div[2]/div/div/button[2]').click()

browser.close()
quit()