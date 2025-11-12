import glob
import os
from langchain.document_loaders import DirectoryLoader, TextLoader, PyPDFLoader

def load_prompt(file_path: str) -> str:
    """
    Load Prompt from .md or .txt file in Specified Path
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            prompt_content = f.read()
        return prompt_content
    except Exception as e:
        print(f"Error when Loading System Prompt : {e}")
        return ""

def load_documents(base_dir: str):
    documents = []
    text_folders = glob.glob(os.path.join(base_dir, "*"))
    # .md, .txt
    for folder in text_folders:
        loader = DirectoryLoader(folder, glob="**/*.*", loader_cls=TextLoader, loader_kwargs={"encoding":"utf-8"})
        try:
            folder_documents = loader.load()
        except Exception:
            folder_documents = []
        for folder_document in folder_documents:
            folder_document.metadata["doc_type"] = os.path.basename(folder)
            documents.append(folder_document)

    # PDF
    for pdf_path in glob.glob(os.path.join(base_dir, "**/*.pdf"), recursive=True):
        try:
            pdf_loader = PyPDFLoader(pdf_path)
            pdf_docs = pdf_loader.load()
            for d in pdf_docs:
                d.metadata["source"] = os.path.basename(pdf_path)
            documents.extend(pdf_docs)
        except Exception as e:
            print("PDF load error", pdf_path, e)
    return documents