from selenium import webdriver
from selenium.webdriver.common.by import By

# Set the path to the WebDriver executable
# Replace 'chromedriver.exe' with the name of the WebDriver you downloaded
# webdriver_path = '/usr/bin/chromedriver'

# URL of the website you want to scrape
url = "https://pcmag.infogram.com/1pegzn0n7j6lgmtm9r7x95nwm9aln1qdq53"

# Initialize the Chrome WebDriver
# driver = webdriver.Chrome(executable_path=webdriver_path)
driver = webdriver.Chrome()

# Open the website
driver.get(url)

# Wait for JavaScript to load (you may need to adjust the waiting time)
driver.implicitly_wait(10)

# Extract data using Selenium
# For example, find an element by its CSS selector
element = driver.find_element(By.CSS_SELECTOR, 'your-css-selector')

# Extract text from the element
text = element.text
print(text)

# Close the WebDriver
driver.quit()
