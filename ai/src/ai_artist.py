# # AI Artist

# Here we have a chatbot where we have added the ability to call functions, now known as tools in OpenAI. Using the sample functions already implemented in DeepLearning.ai as template, several additional functions are implemmented.
# 1.  ```write_file``` gives the ability to write a file that the LLM decides to write out.
# 2.  ```run_chdir``` allows a change of directory
# 3.  ```run_cmd``` adds the ability to run any program in non-shell mode.
# 4.  ```run_quartus_syn``` adds the ability to run Quartus synthesis.
# 5.  ```run_quartus_fit``` adds the ability to run Quartus fitter.
# 6.  ```run_quartus_sta``` adds the ability to run Quartus static timing analysis.
# 7.  ```run_quartus_asm``` adds the ability to run Quartus assembler.
# 8.  What I want to experiment is the ability to generate qsf:
#     1. Add the ability to generate simple .qsf file for Intel's Quartus Prime Software.
#     2. Build a list of these QSF variables in memory
#     3. Have the LLM write out the list
# 9.  Use the LLM to generate a Verilog testcase
# 10. Use the LLM to generate an SDC file
# 11. Use the LLM to launch Quartus compile

# ### Running Intel's Quartus Components
# The list of commands shown below are extracted from a typical Quartus compile run.
# ```
# f'quartus_syn --ipc_flow=17 --ipc_mode --read_settings_files=on --write_settings_files=off {project} -c {project}'
# f'quartus_fit --ipc_flow=17 --ipc_mode --read_settings_files=on --write_settings_files=off {project} -c {project}'
# f'quartus_sta --ipc_flow=17 --ipc_mode {project} -c {project} --mode=finalize'
# f'quartus_asm --ipc_flow=17 --ipc_mode --read_settings_files=on --write_settings_files=off {project} -c {project}'
# f'quartus_sta --ipc_sh'
# ```

# ## OpenAI Setup

import os
import shlex
import sys
import subprocess
from langchain_openai import OpenAI
from openai import OpenAIError

# ## Import libraries
from langchain.tools import tool

import requests
from pydantic.v1 import BaseModel, Field
import datetime

from langchain_openai import ChatOpenAI
from langchain.prompts import ChatPromptTemplate
from langchain_core.utils.function_calling import convert_to_openai_function
from langchain.agents.output_parsers import OpenAIFunctionsAgentOutputParser

from langchain.memory import ConversationBufferMemory
from langchain.prompts import MessagesPlaceholder
from langchain.schema.runnable import RunnablePassthrough
from langchain.schema.agent import AgentFinish
from langchain.agents.format_scratchpad import format_to_openai_functions
from langchain.agents import AgentExecutor

import param

# ## Tools

# ### Function: write_file
# Define the input schema
class FileWriter(BaseModel):
    filename: str = Field(..., description="Name of the file")
    contents: str = Field(..., description="Contents of the file")

@tool(args_schema=FileWriter)
def write_file(filename: str, contents: str) -> str:
    """This function writes out a file using the given filename, creating directories in that process"""

    try:
        with open(filename, 'w') as f:
            f.write(contents)
    except OSError as error:
        try:
            os.makedirs(os.path.dirname(filename), exist_ok=True)
            with open(filename, 'w') as f:
                f.write(contents)
        except OSError as error:
            print(f'{error}')
            return f'{error}'
    
    return f'{filename} {contents}'


# ### Function: run_chdir
# Define the input schema
class RunDirectoryCommand(BaseModel):
    directory: str = Field(..., description="Name of the directory")
    result: str = Field(..., description="Result of the directory command")

@tool(args_schema=RunDirectoryCommand)
def run_chdir(directory: str, result: str) -> str:
    """This function executes change directory and returns the result"""

    work_dir = os.getcwd()
    try:
        os.chdir(directory)
    except OSError as error:
        print(f'{error}')
        return f'{error}'

    return f'{work_dir} {directory}'


# ### Function: run_cmd
def _run_subprocess(command: str, result: str) -> str:    
    cmd = shlex.split(command)
    try:
        pipe = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        result, stderr = pipe.communicate()
        pipe.wait()
    except OSError as error:
        print(f'{error}')
        return f'{error}'
    
    return f'{command} {result} {stderr}'

# Define the input schema
class RunCommand(BaseModel):
    command: str = Field(..., description="Name of the command")
    result: str = Field(..., description="Result of the command")

@tool(args_schema=RunCommand)
def run_cmd(command: str, result: str) -> str:
    """This function executes the specified command and returns the result"""
    
    return _run_subprocess(command, result)

# ### Function: run_quartus_syn
# Define the input schema
class RunQuartus(BaseModel):
    project: str = Field(..., description="Name of the project")
    result: str = Field(..., description="Result of running project")

@tool(args_schema=RunQuartus)
def run_quartus_syn(project: str, result: str) -> str:
    """This function executes the quartus synthesis if quartus pro is installed with the given project and returns the result. 
    This function accepts the project file base name, without the extension.
    A quartus project is a Quartus Settings file with extension qsf.
    Qsf lists all the necessary variables essential to run a Quartus compile.
    This is the primary first step in the quartus compile process if quartus pro is installed.
    Synthesis step generates a netlist friendly to FPGA fabric from the user's RTL description in HDL languages.
    It takes in a design in RTL form and generates an intermediate database."""
    
    command = f'quartus_syn --ipc_flow=17 --ipc_mode --read_settings_files=on --write_settings_files=off {project} -c {project}'
    
    return _run_subprocess(command, result)


# ### Function: run_quartus_map
# Define the input schema
class RunQuartus(BaseModel):
    project: str = Field(..., description="Name of the project")
    result: str = Field(..., description="Result of running project")

@tool(args_schema=RunQuartus)
def run_quartus_map(project: str, result: str) -> str:
    """This function executes the quartus synthesis if quartus lite is installed, with the given project and returns the result. 
    This function accepts the project file base name, without the extension.
    A quartus project is a Quartus Settings file with extension qsf.
    Qsf lists all the necessary variables essential to run a Quartus compile.
    This is the alternate first step in the quartus compile process if quartus lite is installed. This should be tried if primary first step is not allowed.
    Synthesis step generates a netlist friendly to FPGA fabric from the user's RTL description in HDL languages.
    It takes in a design in RTL form and generates an intermediate database."""
    
    command = f'quartus_map --ipc_flow=17 --ipc_mode --read_settings_files=on --write_settings_files=off {project} -c {project}'
    
    return _run_subprocess(command, result)


# ### Function: run_quartus_fit
# Define the input schema
class RunQuartus(BaseModel):
    project: str = Field(..., description="Name of the project")
    result: str = Field(..., description="Result of running project")

@tool(args_schema=RunQuartus)
def run_quartus_fit(project: str, result: str) -> str:
    """This function executes the quartus fitter with the given project and returns the result. 
    This function accepts the project file base name, without the extension.
    A quartus project is a Quartus Settings file with extension qsf.
    Qsf lists all the necessary variables essential to run a Quartus compile.
    This is the second step in the quartus compile process.
    This step can only be run if quartus synthesis was successful.
    Fitter essentially runs placement and routing of the design on the chosen FPGA.
    It takes in database generated by quartus synthesis and updates the database."""
    
    command = f'quartus_fit --ipc_flow=17 --ipc_mode --read_settings_files=on --write_settings_files=off {project} -c {project}'
    
    return _run_subprocess(command, result)


# ### Function: run_quartus_sta
# Define the input schema
class RunQuartus(BaseModel):
    project: str = Field(..., description="Name of the project")
    result: str = Field(..., description="Result of running project")

@tool(args_schema=RunQuartus)
def run_quartus_sta(project: str, result: str) -> str:
    """This function executes the quartus assembler with the given project and returns the result. 
    This function accepts the project file base name, without the extension.
    A quartus project is a Quartus Settings file with extension qsf.
    Qsf lists all the necessary variables essential to run a Quartus compile.
    This is the third step in the quartus compile process.
    This step can only be run if quartus fitter was successful.
    Static Timing Analysis calculates the delays across the design and validates them against the user constraints.
    It takes in database generated by quartus fitter and 
    runs static timing analysis or sta to determine the goodness of the results"""
    
    command = f'quartus_sta --ipc_flow=17 --ipc_mode {project} -c {project} --mode=finalize'
    
    return _run_subprocess(command, result)

# ### Function: run_quartus_asm
# Define the input schema
class RunQuartus(BaseModel):
    project: str = Field(..., description="Name of the project")
    result: str = Field(..., description="Result of running project")

@tool(args_schema=RunQuartus)
def run_quartus_asm(project: str, result: str) -> str:
    """This function executes the quartus assembler with the given project and returns the result. 
    This function accepts the project file base name, without the extension.
    A quartus project is a Quartus Settings file with extension qsf.
    Qsf lists all the necessary variables essential to run a Quartus compile.
    This is the fourth step in the quartus compile process.
    This step can only be run if quartus fitter was successful.
    Assembler's job is to generate a bitstrem that is used to program the chosen FPGA.
    It takes in database generated by quartus fitter and generates the final bitstream."""
    
    command = f'quartus_asm --ipc_flow=17 --ipc_mode --read_settings_files=on --write_settings_files=off {project} -c {project}'
    
    return _run_subprocess(command, result)

# ## Build ChatBot
class AIArtist(param.Parameterized):
    model_name = param.String(default="gpt-3.5-turbo", doc="Open AI model name")
    
    def __init__(self, **params):
        super(AIArtist, self).__init__( **params)

        self.title = 'Ai Artist'
        
        # Build tool list
        self.tools = self.get_tools()
        
        # Build converted function list
        self.functions = self.get_functions()

        # Get OpenAI model
        self.model = self.get_model()

        # Get conversation buffer
        self.memory = self.get_memory_buffer()

        # Build prompt
        self.prompt = self.get_prompt()

        # Create chain
        self.chain = self.get_chain()

        # Create executer agent
        self.qa = self.get_agent()

    def get_tools(self):
        return [write_file, 
                run_chdir, 
                run_cmd, 
                run_quartus_syn,
                run_quartus_map,
                run_quartus_fit,
                run_quartus_sta,
                run_quartus_asm]

    def get_functions(self):
        return [convert_to_openai_function(f) for f in self.tools]

    def get_model(self):
        return ChatOpenAI(model_name=self.model_name, temperature=0).bind(functions=self.functions)

    def get_memory_buffer(self):
        return ConversationBufferMemory(return_messages=True,memory_key="chat_history")

    def get_prompt(self):
        return ChatPromptTemplate.from_messages([
            ("system", "You are helpful but sassy assistant"),
            MessagesPlaceholder(variable_name="chat_history"),
            ("user", "{input}"),
            MessagesPlaceholder(variable_name="agent_scratchpad")
        ])

    def get_chain(self):
        return RunnablePassthrough.assign(
            agent_scratchpad = lambda x: format_to_openai_functions(x["intermediate_steps"])
        ) | self.prompt | self.model | OpenAIFunctionsAgentOutputParser()

    def get_agent(self):
        return AgentExecutor(agent=self.chain, 
                             tools=self.tools, 
                             verbose=False, 
                             memory=self.memory, 
                             handle_parsing_errors=True)
    
    def ask(self, query):
        try:
            result = self.qa.invoke({"input": query})
            self.answer = result['output']
        except OpenAIError as error:
            self.answer = (f"Error: {error}")
        return self.answer
    
    def clr_history(self,count=0):
        self.chat_history = []
        return 
    
    def general_tips(self):
        return """
            <h1>Sample Q&A with AI Artist</h1>
            <ol>
                <li>Hi, my name is Rohit!</li>
                <li>What's my name?</li>
                <li>What tools do you have available?</li>
                <li>Can you write files?</li>
                <li>Generate a dual port RAM in verilog and write a file with the name <b>demo/dual_port_ram.v</b></li>
                <li>Generate Quartus's qsf file with name <b>demo/dual_port_ram.qsf</b> with family Stratix 10 and top level as dual_port_ram</li>
                <li>Run quartus synthesis with project as <b>demo/dual_port_ram</b></li>
                <li>Run quartus fitter with project as <b>demo/dual_port_ram</b></li>
                <li>Run quartus sta with project as <b>demo/dual_port_ram</b></li>
                <li>Run quartus assembler with project as <b>demo/dual_port_ram</b></li>
                <li>Write a unix script to find files matching a given pattern. Name the script as <b>demo/find_files.sh</b></li>
                <li>Do you know what is a DSP?</li>
                <li>Can you implement a DSP example using verilog and write it to a file <b>demo/dsp.v?</li>
            </ol>
        """

    def quartus_tips(self):
        return """
            <h1>Try Quartus Compile</h1>
            <ol>
                <li>Do you know how to run Quartus Compile?</li>
                <li>Can you list the steps in Quartus compile?</li>
                <li>What is the current directory?</li>
                <li>Please create a new directory called <b>../test/designs/dual_port_ram/runs/run_by_ai</b></li>
                <li>Please change directory to <b>../test/designs/dual_port_ram/runs/run_by_ai</b>.</li>
                <li>What is the current directory? Please list full path.</li>
                <li>Please list the files in <b>../../project</b>.</li>
                <li>Please copy quartus project file <b>../../project/dual_port_ram.qsf</b> to current directory.</li>
                <li>Please list files in current directory.</li>
                <li>Please run Quartus compile on project dual_port_ram, listing each step as you are running them.</li>
            </ol>
        """