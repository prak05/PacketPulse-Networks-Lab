import streamlit as st # Web framework for UI 🌐
import google.generativeai as genai # Google AI SDK 🧠
from llama_index.llms.gemini import Gemini # Gemini LLM connector 🔗
from llama_index.embeddings.gemini import GeminiEmbedding # Gemini Embeddings 🔢
from llama_index.core import VectorStoreIndex, Settings # RAG Orchestration 🏗️
from llama_index.readers.file import PDFReader # PDF Parser 📄
import os # OS utilities 💻

# Source: https://docs.llamaindex.ai/en/stable/examples/llm/gemini/
# Source: https://ai.google.dev/gemini-api/docs/quickstart

def initialiazation():
    """Sets up Gemini and UI configuration."""
    # Authenticate using the secret key
    genai.configure(api_key=st.secrets["GOOGLE_API_KEY"])
    # Configure Gemini 1.5 Flash for the LLM
    Settings.llm = Gemini(model_name="models/gemini-1.5-flash")
    # Configure Gemini's native embedding model
    Settings.embed_model = GeminiEmbedding(model_name="models/embedding-001")
    st.set_page_config(page_title="MedBuddy Gemini")
    st.title("MedBuddy: Gemini RAG System 🏥")

def data_ingestion(uploaded_file):
    """pdfreader & vectorbox: Processes PDF into a vector index."""
    # Save file temporarily to disk
    with open("temp_med.pdf", "wb") as f:
        f.write(uploaded_file.getbuffer())
    
    # pdfreader: Load the medical text
    loader = PDFReader()
    documents = loader.load_data(file="temp_med.pdf")
    
    # vectorbox: Create the vector index (embeddings)
    index = VectorStoreIndex.from_documents(documents)
    return index

def promptsboi(query, index):
    """Refines the query and retrieves context."""
    # Setup the retrieval engine
    query_engine = index.as_query_engine()
    # response: Generate the grounded answer
    response_obj = query_engine.query(query)
    return response_obj.response

def mediator():
    """Coordinates the UI flow and logic."""
    initialiazation()
    
    # user: Sidebar upload
    uploaded_file = st.sidebar.file_uploader("Upload Medical PDF", type="pdf")
    # user: Chat input
    user_query = st.chat_input("Ask a clinical question...")

    if uploaded_file and user_query:
        # Process and Output
        index = data_ingestion(uploaded_file)
        answer = promptsboi(user_query, index)
        st.write(f"**MedBuddy:** {answer}")

    # bye bye bye: Cleanup
    if st.sidebar.button("Reset Session"):
        st.cache_resource.clear()
        st.rerun()

if __name__ == "__main__":
    mediator()
