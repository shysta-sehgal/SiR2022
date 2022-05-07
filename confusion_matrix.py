"""
This module produces the confusion matrix for the Chomsky-Halle features dataset and the HS99 dataset. It also finds the
correlation of the confusability values generated for phoneme pairings that are the same in both the datasets.

Precondition: The dataset is in a .txt file in the format described below:
The first row is formatted in the following manner always: [Features, phoneme_1, phoneme_2,..., phoneme_k]
The subsequent rows are formatted in the following manner always: [{feature_name}, value of either -1, 1, or
0 for phoneme_1, value for phoneme_2,..., value for phoneme_k]
"""
import pathlib
import os
from typing import Union
import pandas as pd
import numpy as np
from numpy import ndarray
from pandas import DataFrame


def read_file(fil: str) -> str:
    """
    This function reads a file to check if the length of each row is the same in the dataset. This helps ensure there is
    no missing data or extra data in the file.
    :param fil: a .txt file that contains the phonemes and their features as values +1, -1, 0
    :return: a string that gives the message if the file is properly formatted or not
    """
    with open(fil) as f:
        line = f.readline()
        arr_0 = line.split(",")
        length = len(arr_0)  # this is the length of the first row
        while line != '':
            arr_1 = line.split(",")
            if len(arr_1) != length:  # if the length of any row is not equal to the first row
                return "Problem with data"
            line = f.readline()
    return "No problems with prelim analysis"


def get_phonemes(fil: str) -> list[str]:
    """
    This function gets the list of phonemes from the file and stores them in an array.
    :param fil: a .txt file that contains the phonemes and their features as values +1, -1, 0
    :return: a list containing phoneme strings
    """
    with open(fil) as f:
        line = f.readline()
        arr_0 = line.split(",")  # split data by phoneme values
        arr_0.remove(arr_0[0])  # remove 'Features' string from the list
        for k in range(len(arr_0)):
            arr_0[k] = arr_0[k].strip()  # remove all whitespaces from the phonemes
    return arr_0


def get_file_length(fil: str) -> int:
    """
    This function returns the number of total features in the .txt file.
    :param fil: a .txt file that contains the phonemes and their features as values +1, -1, 0
    :return: an integer value of the total number of features in the file
    """
    with open(fil) as f:
        nonempty_lines = [line.strip("\n") for line in f if line != "\n"]
    line_count = len(nonempty_lines)
    return line_count


def get_features(fil: str) -> list[list[str]]:
    """
    This function returns the feature value for each phoneme for all the features in the file.
    :param fil: a .txt file that contains the phonemes and their features as values +1, -1, 0
    :return: a list containing a list of values. Each nested list contains feature values for each phoneme for one
    feature.
    """
    with open(fil) as f:
        f.readline()  # skip the first line as it contains phoneme names only
        line = f.readline()
        temp = []
        while line != '':
            feature = line.split(",")
            for k in range(len(feature)):
                feature[k] = feature[k].strip()
            temp.append(feature)
            line = f.readline()
        return temp


def feature_vector(feature_list: list[list[str]], phoneme_list: list[str]) -> dict[str, list[str]]:
    """
    This function returns a dictionary such that each phoneme is a key and its value is a list of all its feature values
    :param feature_list: a list containing lists of feature values for each phoneme
    :param phoneme_list: a list containing all phoneme names
    :return: a dictionary in the following format : {phoneme_1: [feature values], phoneme_2: [feature values],...,
    phoneme_n: [feature values]}
    """
    phoneme_dic = {}
    for k in range(len(phoneme_list)):
        phoneme_dic[phoneme_list[k]] = []
        for j in range(len(feature_list)):
            phoneme_dic[phoneme_list[k]].append(feature_list[j][k + 1])
    return phoneme_dic


def get_overlap(phoneme_dic: dict[str, list[str]]) -> list[Union[list[list[str]], list[int]]]:
    """
    This function finds the overlap value for each phoneme pairing in the data, i.e., the number of feature values that
    each phoneme pairing has in common.
    :param phoneme_dic: a dictionary that contains phonemes as keys and a list of featural values as its value
    :return: a list containing a list of phoneme pairings and a list containing overlap values
    """
    phoneme_pairings = []
    overlap_values = []
    temp = []  # list to store phoneme pairings that have already been stored in the dictionary to avoid duplicates
    for key in phoneme_dic:
        for key2 in phoneme_dic:
            if key2 != key and [key, key2] not in temp and [key2, key] not in temp:  # check point to avoid adding same
                # keys and phoneme pairings already in temp
                j = 0  # counter for overlap values between each phoneme
                for k in range(len(phoneme_dic[key])):
                    if phoneme_dic[key][k] == phoneme_dic[key2][k]:
                        j += 1
                phoneme_pairings.append([key, key2])  # add phoneme pair to the list
                overlap_values.append(j)  # store overlap values at the corresponding index in a different list

                # store the phoneme pairs already analysed in temp
                temp.append([key, key2])
                temp.append([key2, key])
    return [phoneme_pairings, overlap_values]


def confusion_matrix(phoneme_tuple_dic: dict[tuple, int]) -> DataFrame:
    """
    This function generates the confusion matrix for the dataset. A confusion matrix is a matrix that tells how
    confusable a phoneme pairing is. This is done by finding either the overlap scores or the correlation scores for
    each phoneme pairing.
    :param phoneme_tuple_dic: A dictionary containing tuples of phoneme pairs as keys and overlap / correlation scores
    as values
    :return: a confusion matrix dataframe
    """
    first_col = []
    first_row = []
    for key in phoneme_tuple_dic:
        first_col.append(key[0])  # add the first phoneme in the tuple to the first column
        first_row.append(key[1])  # add the second phoneme in the tuple to the first row

    # remove duplicates from both the lists
    phoneme_1 = list(dict.fromkeys(first_col))
    phoneme_2 = list(dict.fromkeys(first_row))

    # make a pandas dataframe with the first column and the first row
    confusion_mat = pd.DataFrame(list(phoneme_1), columns=['phonemes'])
    confusion_mat = confusion_mat.reindex(columns=confusion_mat.columns.tolist() + phoneme_2)

    # fill in the dataframe with the normalised overlap / correlation values
    for col in confusion_mat.columns[1:]:
        for k in range(len(phoneme_1)):
            if (confusion_mat["phonemes"][k], col) in phoneme_tuple_dic:
                confusion_mat.at[k, col] = phoneme_tuple_dic[confusion_mat["phonemes"][k], col]
    return confusion_mat


def normalised_dataframe(files: list[str]) -> list[DataFrame]:
    """
    This function returns a list containing dataframes and stores it in a .csv file. The dataframe contains phoneme
    pairings and their normalised values.
    :param files: a list containing .txt files from which to read data
    :return: a list of dataframes that contain phoneme pairings and their normalised confusability values.
    """
    tracker = []  # a list to store the dataframes generated for both datasets
    for file in files:
        line_count = get_file_length(file)
        feature_lis = get_features(file)  # get the feature list for the phonemes
        phoneme_lis = get_phonemes(file)  # get the list of phonemes from the file
        dictionary = feature_vector(feature_lis, phoneme_lis)  # get the feature vectors for the phonemes
        overlap_arr = get_overlap(dictionary)  # get the overlap values for the phoneme pairings
        # make a dataframe with phoneme pairings column and overlap / correlation values column
        df = pd.DataFrame(list(zip(overlap_arr[0], overlap_arr[1])), columns=['phonemes', 'overlap'])
        overlap_array = list(df['overlap'].copy())

        # normalise the overlap / correlation values
        for j in range(len(overlap_array)):
            overlap_array[j] = round(overlap_array[j] / line_count, 2)
        df['overlap'] = overlap_array  # change the overlap column of the dataframe to normalised values
        tracker.append(df)  # add the dataframe to the tracker

        # output filename
        dot_index = file.index(".")
        file_name = file[:dot_index + 1] + 'csv'
        parent_dir = pathlib.Path(__file__).parent

        # directory for output file
        directory = "Results"
        path = os.path.join(parent_dir, directory)
        if not os.path.isdir(path):
            os.mkdir(path)
        tracker[files.index(file)].to_csv(path+"/"+file_name)  # store the dataframe in a .csv format
    return tracker


def find_correlation(file1: str, file2: str) -> tuple[ndarray, str]:
    """
    This functions finds the phoneme pairs that are the same in the two .txt files and reports the correlation between
    either the overlap values or the correlation values for those pairs
    :param file1: a .txt file that contains the phonemes and their features as values +1, -1, 0
    :param file2: a .txt file that contains the phonemes and their features as values +1, -1, 0
    :return: a tuple containing the numpy array with the correlation values and the number of phoneme pairs that the
    two files have in common
    """
    tracker = normalised_dataframe([file1, file2])
    similarity_list = {}  # dictionary with phoneme pairings and normalised values in the dataset for the given files
    phoneme_1_copy = list(tracker[0]["phonemes"].copy())
    phoneme_2_copy = list(tracker[1]["phonemes"].copy())
    overlap_1_copy = list(tracker[0]["overlap"].copy())
    overlap_2_copy = list(tracker[1]["overlap"].copy())
    corr_1 = []
    corr_2 = []
    common_pairs = 0
    # check what phoneme pairs are common across both dataframes
    for j in range(len(phoneme_1_copy)):
        x = tuple(phoneme_1_copy[j])
        if phoneme_1_copy[j] in phoneme_2_copy:
            y = phoneme_2_copy.index(phoneme_1_copy[j])
            similarity_list[x] = (overlap_1_copy[j], overlap_2_copy[y])  # copy the overlap values for the phoneme pairs
            # in both dataframes

            # store overlap values in different lists at corresponding indices for correlation analysis
            corr_1.append(overlap_1_copy[j])
            corr_2.append(overlap_2_copy[y])
            common_pairs += 1
        else:
            new_lis = [x[1], x[0]]
            new_tup = (x[1], x[0])
            if new_lis in phoneme_2_copy:
                y = phoneme_2_copy.index(new_lis)
                similarity_list[new_tup] = (overlap_1_copy[j], overlap_2_copy[y])  # copy the overlap values for the
                # phoneme pair in both dataframes

                # store overlap values in different lists at corresponding indices for correlation analysis
                corr_1.append(overlap_1_copy[j])
                corr_2.append(overlap_2_copy[y])
                common_pairs += 1
    return np.corrcoef(corr_1, corr_2), "The number of common_pairs are " + str(common_pairs)


def make_conf_matrix(file: str) -> None:
    """
    This function returns the confusion matrix with the normalised correlation or overlap values between phoneme pairs
    in a .csv file.
    :param file: a .txt file that contains the phonemes and their features as values +1, -1, 0
    :return: None
    """
    df_tracker = normalised_dataframe([file])
    phoneme_copy = list(df_tracker[0]["phonemes"].copy())
    overlap_copy = list(df_tracker[0]["overlap"].copy())
    matrix = {}
    for i in range(len(phoneme_copy)):
        tup_matrix = tuple(phoneme_copy[i])
        matrix[tup_matrix] = overlap_copy[i]
    confusion = confusion_matrix(matrix)
    dot_index = file.index(".")
    file_name = file[:dot_index] + '_conf_matrix.csv'
    parent_dir = pathlib.Path(__file__).parent
    directory = "Results"
    path = os.path.join(parent_dir, directory)
    if not os.path.isdir(path):
        os.mkdir(path)
    confusion.to_csv(path + '/' + file_name)


if __name__ == "__main__":
    make_conf_matrix("HS99.txt")  # make a confusion matrix and output to csv for any .txt file in the specified format

    # find the correlation between overlap values or correlation values between two .txt files
    print(find_correlation("HS99.txt", "CH68.txt"))

    # normalise the overlap or correlation values for phoneme pairs and output it to .csv file for a list of .txt files
    normalised_dataframe(["CH68.txt", "HS99.txt"])
