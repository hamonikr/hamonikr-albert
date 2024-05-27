# -*- coding: utf-8 -*-

"""Query OpenAI using the Python 'requests' library.

Synopsis: <trigger> <query>"""

import json
import urllib.request
import configparser
from pathlib import Path
from time import sleep
from albert import *

__title__ = "OpenAI Query"
__version__ = "0.4.0"
__triggers__ = "ai> "  # trigger
__authors__ = "Kevin Kim"

iconPath = str(Path(__file__).parent / "openai.svg")
API_URL = "https://api.openai.com/v1/chat/completions"
MODEL = "gpt-3.5-turbo"
CONFIG_PATH = Path.home() / '.config/albert/albert.conf'

def read_api_key():
    config = configparser.ConfigParser()
    config.read(CONFIG_PATH)
    return config.get('org.albert.extension.python', 'openai_api_key', fallback=None)

api_key = read_api_key()
_query_timer = None
_query_result = None

def handleQuery(query):
    if query.isTriggered:
        stripped = query.string.strip()
        item = Item(id=__title__, icon=iconPath)

        if api_key is None:
            item.text = "API key is not set."
            item.subtext = "Please set the API key in the configuration file."
            return item

        if len(stripped) < 3:
            item.text = "Query too short"
            item.subtext = "Please provide a longer query."
            return item

        # Placeholder for loading
        item.text = "Waiting for OpenAI response..."
        item.subtext = "Your query is being processed."
        
        sleep(1)  # 1초 대기

        headers = {
            "Content-Type": "application/json",
            "Authorization": f"Bearer {api_key}"
        }
        data = {
            "model": MODEL,
            "messages": [{"role": "user", "content": stripped}],
            "max_tokens": 100
        }

        req = urllib.request.Request(API_URL, headers=headers, data=json.dumps(data).encode('utf-8'))
        try:
            with urllib.request.urlopen(req) as response:
                response_data = json.loads(response.read().decode('utf-8'))
                result = response_data['choices'][0]['message']['content'].strip()
                item.text = result
                item.subtext = f"OpenAI response to: {stripped}"
                item.addAction(ClipAction("Copy response to clipboard", result))
                return item
        except Exception as e:
            item.text = "Error"
            item.subtext = str(e)
            return item
