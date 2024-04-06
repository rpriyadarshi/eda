# # AI Artist panel GUI
# https://stackoverflow.com/questions/2349991/how-do-i-import-other-python-files

import panel as pn  # GUI
pn.extension()
from ai_artist import *

# ## Create and render ChatBot
class AIAssistant():

    def __init__(self):
        self.ai = AIArtist()
        chat_interface = pn.chat.ChatInterface(callback=self.callback, callback_user="AI Artist")
        chat_interface.servable()

    async def callback(self, query: str, user: str, instance: pn.chat.ChatInterface):
        answer = self.ai.ask(query)
        yield answer

ai = AIAssistant()

# ### Serve the panel
# panel serve pn_gui.py --autoreload
