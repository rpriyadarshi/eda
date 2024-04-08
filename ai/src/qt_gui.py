# https://www.youtube.com/watch?v=abAbSsZnUS0
# https://dev.to/m_herrmann/python-qt-tutorial-create-a-simple-chat-client-cgl
# https://realpython.com/python-pyqt-layout/
# https://www.pythonguis.com/tutorials/pyside6-layouts/

import sys
from functools import partial

from PySide6.QtWidgets import QApplication, QWidget, QVBoxLayout, QTabWidget, QGridLayout, QLabel, QMainWindow, QTextEdit, QLineEdit, QPushButton, QMenuBar, QMenu, QToolBar, QSpinBox
from PySide6.QtCore import Qt
from PySide6.QtGui import QIcon, QAction, QKeySequence, QPalette, QColor

# Icons are in: /usr/share/icons/Humanity/actions/48
# pyside6-rcc -o qrc_resources.py resources.qrc
import qrc_resources

from ai_artist import *

class Color(QWidget):

    def __init__(self, color):
        super(Color, self).__init__()
        self.setAutoFillBackground(True)

        palette = self.palette()
        palette.setColor(QPalette.Window, QColor(color))
        self.setPalette(palette)

class AIAssistant(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ai = AIArtist()

        self.title = self.ai.title
        self.mainWidth = 1200
        self.mainHeight = 800

    def _setupMainWindow(self):
        self.setWindowTitle(self.title)
        self.setMinimumSize(self.mainWidth, self.mainHeight)

    def _buildCentralWidget(self):
        self.centralWidget = QTabWidget()
        self.centralWidget.setTabPosition(QTabWidget.West)
        self.centralWidget.setMovable(True)
        self.centralWidget.setUsesScrollButtons(True)
        self.centralWidget.setDocumentMode(True)

        self.centralWidget.addTab(self.chatWidget, self.ai.title)
        self.centralWidget.addTab(self.generalTipsWidget, "General Tips")
        self.centralWidget.addTab(self.quartusTipsWidget, "Quartus Tips")

        self.setCentralWidget(self.centralWidget)

    def _createActions(self):
        # Creating action using the first constructor
        self.newAction = QAction(QIcon(":filenew.svg"), "&New", self)
        self.openAction = QAction(QIcon(":fileopen.svg"), "&Open...", self)
        self.saveAction = QAction(QIcon(":filesave.svg"), "&Save", self)
        self.saveAsAction = QAction(QIcon(":filesaveas.svg"), "&SaveAs...", self)
        self.printAction = QAction(QIcon(":fileprint.svg"), "&Print...", self)
        self.exitAction = QAction(QIcon(":exit.svg"), "&Exit", self)

        # Using standard keys
        self.newAction.setShortcut(QKeySequence.New)
        self.openAction.setShortcut(QKeySequence.Open)
        self.saveAction.setShortcut(QKeySequence.Save)
        self.saveAsAction.setShortcut(QKeySequence.SaveAs)
        self.printAction.setShortcut(QKeySequence.Print)

        # Adding help tips
        self.newTip = "Create a new file"
        self.newAction.setStatusTip(self.newTip)
        self.newAction.setToolTip(self.newTip)

        # Edit actions
        self.copyAction = QAction(QIcon(":edit-copy.svg"), "&Copy", self)
        self.pasteAction = QAction(QIcon(":edit-paste.svg"), "&Paste", self)
        self.cutAction = QAction(QIcon(":edit-cut.svg"), "C&ut", self)
        self.deleteAction = QAction(QIcon(":edit-delete.svg"), "&Delete", self)
        self.redoAction = QAction(QIcon(":edit-redo.svg"), "Red&o", self)
        self.undoAction = QAction(QIcon(":edit-undo.svg"), "&Undo", self)

        # Using standard keys
        self.copyAction.setShortcut(QKeySequence.Copy)
        self.pasteAction.setShortcut(QKeySequence.Paste)
        self.cutAction.setShortcut(QKeySequence.Cut)
        self.deleteAction.setShortcut(QKeySequence.Delete)
        self.redoAction.setShortcut(QKeySequence.Redo)
        self.undoAction.setShortcut(QKeySequence.Undo)

        self.findAction = QAction("Find...", self)
        self.replaceAction = QAction("Replace...", self)

        self.findAction.setShortcut(QKeySequence.Find)
        self.replaceAction.setShortcut(QKeySequence.Replace)

        # Help actions
        self.helpContentAction = QAction(QIcon(":help-contents.svg"), "&Help Content", self)
        self.aboutAction = QAction(QIcon(":help-about.svg"), "&About", self)

        self.helpContentAction.setShortcut(QKeySequence.HelpContents)

    def _createToolBars(self):
        # Using a title
        self.fileToolBar = self.addToolBar("File")
        self.fileToolBar.addAction(self.newAction)
        self.fileToolBar.addAction(self.openAction)
        self.fileToolBar.addAction(self.saveAction)
        self.fileToolBar.addAction(self.saveAsAction)
        self.fileToolBar.addAction(self.printAction)

        # Using a QToolBar object
        self.editToolBar = QToolBar("Edit", self)
        self.addToolBar(self.editToolBar)
        self.editToolBar.addAction(self.copyAction)
        self.editToolBar.addAction(self.pasteAction)
        self.editToolBar.addAction(self.cutAction)
        self.editToolBar.addAction(self.deleteAction)
        self.editToolBar.addAction(self.redoAction)
        self.editToolBar.addAction(self.undoAction)

        # Adding a widget to the Edit toolbar
        self.fontSizeSpinBox = QSpinBox()
        self.fontSizeSpinBox.setFocusPolicy(Qt.NoFocus)
        self.editToolBar.addWidget(self.fontSizeSpinBox)

        # Using a QToolBar object and a toolbar area
        self.helpToolBar = QToolBar("Help", self)
        self.addToolBar(self.helpToolBar)
        self.helpToolBar.addAction(self.helpContentAction)

    # Event handler override
    def contextMenuEvent(self, event):
        self.separator = QAction(self)
        self.separator.setSeparator(True)
        # Creating a menu object with the central widget as parent
        self.menu = QMenu(self.centralWidget)
        # Populating the menu with actions
        self.menu.addAction(self.newAction)
        self.menu.addAction(self.openAction)
        self.menu.addAction(self.saveAction)
        self.menu.addAction(self.separator)
        self.menu.addAction(self.copyAction)
        self.menu.addAction(self.pasteAction)
        self.menu.addAction(self.cutAction)
        # Launching the menu
        self.menu.exec(event.globalPos())

    def _buildMenuBar(self):
        self.menuBar = QMenuBar(self)
        self.setMenuBar(self.menuBar)

        # File menu
        self.fileMenu = QMenu("&File", self)
        self.menuBar.addMenu(self.fileMenu)
        self.fileMenu.addAction(self.newAction)
        self.fileMenu.addAction(self.openAction)
        self.openRecentMenu = self.fileMenu.addMenu(QIcon(":document-open-recent.svg"), "Open Recent")
        self.fileMenu.addAction(self.saveAction)
        self.fileMenu.addAction(self.saveAsAction)
        self.fileMenu.addSeparator()
        self.fileMenu.addAction(self.printAction)
        self.fileMenu.addSeparator()
        self.fileMenu.addAction(self.exitAction)

        # Edit menu
        self.editMenu = self.menuBar.addMenu("&Edit")
        self.editMenu.addAction(self.copyAction)
        self.editMenu.addAction(self.pasteAction)
        self.editMenu.addAction(self.cutAction)
        self.editMenu.addSeparator()
        self.editMenu.addAction(self.deleteAction)
        self.editMenu.addSeparator()
        self.editMenu.addAction(self.redoAction)
        self.editMenu.addAction(self.undoAction)

        # Find and Replace submenu in the Edit menu
        self.editMenu.addSeparator()
        self.findMenu = self.editMenu.addMenu("Find and Replace")
        self.findMenu.addAction(self.findAction)
        self.findMenu.addAction(self.replaceAction)

        # Help menu
        self.helpMenu = self.menuBar.addMenu("&Help")
        self.helpMenu.addAction(self.helpContentAction)
        self.helpMenu.addAction(self.aboutAction)

    def _populateOpenRecent(self):
        # Step 1. Remove the old options from the menu
        self.openRecentMenu.clear()
        # Step 2. Dynamically create the actions
        actions = []
        filenames = [f"File-{n}" for n in range(5)]
        for filename in filenames:
            action = QAction(filename, self)
            action.triggered.connect(partial(self.openRecentFile, filename))
            actions.append(action)
        # Step 3. Add the actions to the menu
        self.openRecentMenu.addActions(actions)

    def _createStatusBar(self):
        self.statusbar = self.statusBar()
        # Adding a temporary message
        self.statusbar.showMessage("Ready", 3000)
        # Adding a permanent message
        self.wcLabel = QLabel(f"{self.getWordCount()} Words")
        self.statusbar.addPermanentWidget(self.wcLabel)

    def _connectActions(self):
        # Connect File actions
        self.newAction.triggered.connect(self.newFile)
        self.openAction.triggered.connect(self.openFile)
        self.saveAction.triggered.connect(self.saveFile)
        self.saveAsAction.triggered.connect(self.saveAsFile)
        self.printAction.triggered.connect(self.printFile)
        self.exitAction.triggered.connect(self.close)
        # Connect Edit actions
        self.copyAction.triggered.connect(self.copyContent)
        self.pasteAction.triggered.connect(self.pasteContent)
        self.cutAction.triggered.connect(self.cutContent)
        self.deleteAction.triggered.connect(self.deleteContent)
        self.redoAction.triggered.connect(self.redoContent)
        self.undoAction.triggered.connect(self.undoContent)
        self.findAction.triggered.connect(self.findContent)
        self.replaceAction.triggered.connect(self.replaceContent)
        # Connect Help actions
        self.helpContentAction.triggered.connect(self.helpContent)
        self.aboutAction.triggered.connect(self.about)
        # Open recent
        self.openRecentMenu.aboutToShow.connect(self._populateOpenRecent)

    def clickMessage(self, category:str, widgetName:str):
        self.statusbar.showMessage(f"{category} > {widgetName} clicked")

    def newFile(self):
        self.clickMessage('File', 'New')

    def openFile(self):
        self.clickMessage('File', 'Open...')

    def saveFile(self):
        self.clickMessage('File', 'Save')

    def saveAsFile(self):
        self.clickMessage('File', 'SaveAs')

    def printFile(self):
        self.clickMessage('File', 'Print')

    def copyContent(self):
        self.clickMessage('Edit', 'Copy')

    def pasteContent(self):
        self.clickMessage('Edit', 'Paste')

    def cutContent(self):
        self.clickMessage('Edit', 'Cut')

    def deleteContent(self):
        self.clickMessage('Edit', 'Delete')

    def redoContent(self):
        self.clickMessage('Edit', 'Redo')

    def undoContent(self):
        self.clickMessage('Edit', 'Undo')

    def findContent(self):
        self.clickMessage('Edit', 'Find')

    def replaceContent(self):
        self.clickMessage('Edit', 'Replace')

    def helpContent(self):
        self.clickMessage('Help', 'Content...')

    def about(self):
        self.clickMessage('Help', 'About...')

    def openRecentFile(self, filename):
        # Logic for opening a recent file goes here...
        self.statusbar.showMessage(f"{filename} opened")

    def getWordCount(self):
        # Logic for computing the word count goes here...
        return 42

    def _buildChatArea(self):
        self.chatLayout = QGridLayout()
        self.chatArea = QTextEdit()
        self.chatArea.setPlaceholderText('Here is where your chat will appear!')
        self.chatArea.setReadOnly(True)
        self.chatInput = QLineEdit()
        self.chatInput.setClearButtonEnabled(True)
        self.chatInput.setDragEnabled(True)
        self.chatInput.setPlaceholderText('Please ask any question!')
        self.sendButton = QPushButton('Send')
        self.sendButton.clicked.connect(self._sendMessage)
        self.chatInput.returnPressed.connect(self.sendButton.click)

        self.chatLayout.addWidget(self.chatArea, 0, 0)
        self.chatLayout.addWidget(self.chatInput, 1, 0)
        self.chatLayout.addWidget(self.sendButton, 1, 1)

        self.chatWidget = QWidget()
        self.chatWidget.setLayout(self.chatLayout)

    def _buildGeneralTips(self):
        self.generalTipsLayout = QVBoxLayout ()
        self.generalTipsArea = QTextEdit()
        self.generalTipsArea.setReadOnly(True)

        self.generalTipsLayout.addWidget(self.generalTipsArea)

        self.generalTipsWidget = QWidget()
        self.generalTipsWidget.setLayout(self.generalTipsLayout)
        self.generalTipsArea.setText(self.ai.general_tips())

    def _buildQuartusTips(self):
        self.quartusTipsLayout = QVBoxLayout ()
        self.quartusTipsArea = QTextEdit()
        self.quartusTipsArea.setReadOnly(True)

        self.quartusTipsLayout.addWidget(self.quartusTipsArea)

        self.quartusTipsWidget = QWidget()
        self.quartusTipsWidget.setLayout(self.quartusTipsLayout)
        self.quartusTipsArea.setText(self.ai.quartus_tips())

    def _buildUserInput(self, query):
        return f'<p><b>User:</b> {query}</p>'
    
    def _buildAIResponse(self, response):
        return f'<p><b>{self.title}:</b> {response}</p>'

    def _sendMessage(self):
        query = self.chatInput.text().strip()
        self.chatArea.append(self._buildUserInput(query))
        QApplication.setOverrideCursor(Qt.BusyCursor)
        response = self.ai.ask(query)
        QApplication.restoreOverrideCursor()
        self.chatArea.append(self._buildAIResponse(response))
        self.chatInput.clear()

    def _buildGui(self):
        self._setupMainWindow()
        self._buildChatArea()
        self._buildGeneralTips()
        self._buildQuartusTips()
        self._buildCentralWidget()
        self._createActions()
        self._createToolBars()
        self._buildMenuBar()
        self._createStatusBar()
        self._connectActions()

    def run(self):
        self._buildGui()
        self.show()

def main():
    app = QApplication(sys.argv)
    ai = AIAssistant()
    ai.run()
    sys.exit(app.exec())

if __name__ == "__main__":
    main()