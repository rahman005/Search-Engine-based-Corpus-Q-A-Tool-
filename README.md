# Text-Corpus-Q-A-Tool-Integrated-with-ChatGPT
This project is a streamlined question-answering system that extracts keywords from user queries, identifies relevant paragraphs using TF-IDF scoring, and utilizes a LLM like ChatGPT to generate concise and accurate responses based on the retrieved information from a specified corpus.

This Project is a question-answering system that operates in several steps:

**1)Query Input:** The system takes a question from the user related to a specific corpus.

**2)Keyword Extraction:** It scans the question to identify keywords or key phrases that are essential for understanding the query.

**3)Paragraph Retrieval:** Using the identified keywords, the system locates relevant paragraphs within the corpus. It employs techniques like TF-IDF (Term Frequency-Inverse Document Frequency) to score the paragraphs based on their relevance to the query.

**4)Ranking and Selection:** The system ranks the paragraphs based on their scores and selects the top paragraphs that are most likely to contain the answer to the user's question. It typically sends the top 5 paragraphs or fewer, depending on the need and relevance.

**5)Query to ChatGPT:** The selected paragraphs serve as input queries to another system, such as ChatGPT, which generates responses based on the content of the provided paragraphs. The responses are tailored to address the user's original question, utilizing the information extracted from the corpus.

Overall, this project combines natural language processing techniques with information retrieval and generation to provide accurate and relevant answers to user queries based on a specific corpus of information.
