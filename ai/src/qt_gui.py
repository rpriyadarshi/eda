# https://www.youtube.com/watch?v=abAbSsZnUS0
# https://dev.to/m_herrmann/python-qt-tutorial-create-a-simple-chat-client-cgl
import sys
from PyQt6.QtWidgets import QApplication, QMainWindow, QTextEdit, QLineEdit, QPushButton
from ai_artist import *

class AIAssistant(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ai = AIArtist()

        self.title = self.ai.title

        self.widgetGap = 20

        self.mainWidth = 750
        self.mainHeight = 500

        self.chatLeftMargin = 10
        self.chatTopMargin = 10
        self.chatWidth = 600
        self.chatHeight = 350

        self.textLeftMargin = self.chatLeftMargin
        self.textTopMargin = self.chatHeight + self.widgetGap
        self.textWidth = self.chatWidth
        self.textHeight = 40

        self.sendButtonLeftMargin = self.chatWidth + self.widgetGap
        self.sendButtonTopMargin = self.chatHeight + self.widgetGap
        self.sendButtonWidth = 100
        self.sendButtonHeight = self.textHeight

    def setupMainWindow(self):
        self.setWindowTitle(self.title)
        self.setMinimumSize(self.mainWidth, self.mainHeight)

    def buildChatArea(self):
        self.chatArea = QTextEdit(self)
        self.chatArea.setGeometry(self.chatLeftMargin, self.chatTopMargin, self.chatWidth, self.chatHeight)
        self.chatArea.setReadOnly(True)

    def buildQueryBox(self):
        self.textBox = QLineEdit(self)
        self.textBox.setGeometry(self.textLeftMargin, self.textTopMargin, self.textWidth, self.textHeight)

    def buildSendButton(self):
        self.sendButtonName = 'Send'
        self.sendButton = QPushButton(self.sendButtonName, self)
        self.sendButton.setGeometry(self.sendButtonLeftMargin, self.sendButtonTopMargin, self.sendButtonWidth, self.sendButtonHeight)
        self.sendButton.clicked.connect(self.sendMessage)

    def buildUserInput(self, query):
        return f'<p><b>User:</b> {query}</p>'
    
    def buildAIResponse(self, response):
        return f'<p><b>{self.title}:</b> {response}</p>'

    def sendMessage(self):
        query = self.textBox.text().strip()
        self.chatArea.append(self.buildUserInput(query))
        response = self.ai.ask(query)
        self.chatArea.append(self.buildAIResponse(response))
        self.textBox.clear()

    def buildGui(self):
        self.setupMainWindow()
        self.buildChatArea()
        self.buildQueryBox()
        self.buildSendButton()

    def run(self):
        self.buildGui()
        self.show()

def main():
    app = QApplication(sys.argv)
    ai = AIAssistant()
    ai.run()
    sys.exit(app.exec())

if __name__ == "__main__":
    main()