"""
A python script to calculate ROUGE score between the summary output by lexrank
program and the golden summary texts given at the end of each document.

This script summarizes the sentences of each document using lexrank executable
produced by the build script. Then, the output summary is compared to the golden
summary of the document given at its end. The comparison is performed using
ROUGE algorithm. This is done by making both the summaries a single sentence and
comparing the sentences according to Rouge-1, Rouge-2 and Rouge-L schemes.

After ROUGE scores for all documents are found, the average ROUGE score is
calculated by calculating the unweighted average of all precision, recall and
F1 scores. This average ROUGE scores are output at the end.

Usage
-----
python rouge_scores.py <dataset_directory>
"""

import subprocess
import sys
import os
from rouge import Rouge


def extract_summary_from_document(document, n_spaces=4):
    """
    Extract summary text from the given document layout.

    A document layout must be of the following format:

        sentence_1
        sentence_2
        ...
        sentence_N

        summary_sentence_1
        summary_sentence_2
        ...
        summary_sentence_K

    This script returns

        summary_sentence_1    summary_sentence_2    ...    summary_sentence_K

    where each summary sentence contains given number of spaces in between.

    Parameters
    ----------
    document : str
        Document string in the mentioned format.

    n_spaces : int
        Number of spaces to put between two consecutive sentence.

    Returns
    -------
    summary : str
        Summary string where each pair of summary sentences contain given number
        of spaces in between.
    """
    # find the empty line in between document and summary sentences
    empty_line_index = document.rfind('\n\n') + 1

    # extract summary
    summary_start_index = document.find('\n', empty_line_index) + 1
    summary = document[summary_start_index:]

    # each pair of summary sentences contain 4 spaces in between
    summary = summary.replace('\n', ' ' * n_spaces)

    return summary


def get_prog_summary(dataset_dir, doc_id):
    """
    Get the summary text produced by lexrank program.

    This function assumes that lexrank program is in the current directory with
    the script.

    Parameters
    ----------
    dataset_dir : str
        Dataset directory path.

    doc_id : int
        ID of the document

    Returns
    -------
    summary : str
        Summary string produced by lexrank program where each sentence is
        separated by 4 spaces.
    """
    cmd = './lexrank {} {}.txt'.format(dataset_dir, doc_id)
    out = subprocess.check_output(cmd, shell=True).decode('utf-8')

    return extract_summary_from_document(out, n_spaces=4)


def get_gold_summary(dataset_dir, doc_id):
    """
    Get the golden summary text for the given document.

    Parameters
    ----------
    dataset_dir : str
        Dataset directory path.

    doc_id : int
        ID of the document

    Returns
    -------
    summary : str
        Golden standard summary for the given document where each consecutive
        sentence is separated by 4 spaces.
    """
    doc_path = os.path.join(dataset_dir, '{}.txt'.format(doc_id))
    with open(doc_path, 'r') as f:
        out = f.read()

    return extract_summary_from_document(out, n_spaces=4)


def calc_rouge_scores(dataset_dir, num_docs):
    """
    Calculate ROUGE scores (Rouge-1, Rouge-2, Rouge-L) for each document in the
    given directory and return the results as a list.

    The documents in the given directory are assumed to be in the following
    naming format:

        1.txt
        2.txt
        ...
        <num_docs>.txt

    The returned list contains the ROUGE score dictionary of all documents in
    the same order, i.e. the list is of the following format:

        [<scores_for_1.txt>, <scores_for_2.txt>, ...]

    Parameters
    ----------
    dataset_dir : str
        Path to the directory containing the individual documents to summarize
        and calculate ROUGE scores.

    num_docs : int
        Number of documents to summarize in the given directory.

    Returns
    -------
    scores : list
        A list of ROUGE score dictionaries where each dictionary is of the
        following format:

            {
                'rouge-1': {'f':..., 'p':..., 'r':...},
                'rouge-2': {'f':..., 'p':..., 'r':...},
                'rouge-L': {'f':..., 'p':..., 'r':...}
            }
    """
    scores = []
    rouge = Rouge()
    for doc_id in range(1, num_docs + 1):
        prog_summary = get_prog_summary(dataset_dir, doc_id)
        gold_summary = get_gold_summary(dataset_dir, doc_id)

        doc_scores = rouge.get_scores(prog_summary, gold_summary)
        scores += doc_scores

    return scores


def avg_rouge_score(rouge_scores):
    """
    Calculate the average ROUGE scores using all the ROUGE score dictionaries
    in the given score list.

    Average is calculated by summing the results for every individual metric and
    dividing the final sum to the number of score dictionaries in the given
    list.

    Parameters
    ----------
    rouge_scores : list
        List of ROUGE score dictionaries.

    Returns
    -------
    avg_score : dict
        Average of the all ROUGE score dictionaries in the given list.
    """
    rouge_types = ['rouge-1', 'rouge-2', 'rouge-l']
    metrics = ['f', 'p', 'r']
    num_docs = len(rouge_scores)

    # sum every metric
    avg_score = rouge_scores[0]
    for score in rouge_scores[1:]:
        for type in rouge_types:
            for metric in metrics:
                avg_score[type][metric] += score[type][metric]

    # divide every metric to number of elements
    for type in rouge_types:
        for metric in metrics:
            avg_score[type][metric] /= num_docs

    return avg_score


def main():
    """
    Main program which performs the following operations:

        i.   Parse command-line arguments
        ii.  Change program working directory to one above
        iii. Calculate ROUGE score for every document in the given directory
        iv.  Calculate average ROUGE score using the calculated scores
        v.   Print the average ROUGE score dictionary.
    """
    if len(sys.argv) != 2:
        print('Usage: {} <dataset_dir>'.format(sys.argv[0]))
        sys.exit(-1)

    dataset_dir = os.path.abspath(sys.argv[1])
    num_docs = len(os.listdir(dataset_dir))
    os.chdir('..')

    rouge_scores = calc_rouge_scores(dataset_dir, num_docs)
    avg_score = avg_rouge_score(rouge_scores)

    print(avg_score)


if __name__ == '__main__':
    main()
