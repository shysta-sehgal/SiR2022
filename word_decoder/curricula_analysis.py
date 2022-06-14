"""
This module gives out the accuracy with which rules from a curriculum can read accurately the vocabulary of a curriculum
for different months.

Precondition: The curriculum file must be a CSV file with the top row header containing at least 'Lexicality', 'Week',
'Type', 'Orthography', 'Phonology' columns.

Information about what these columns contain can be understood through the
"Jolly Phonics Curriculum Coding Sheet - Coder 1.csv" and "Letters and Sounds Curriculum Coding Sheet - Coder 1.csv"
files. For any further clarifications, please reach out to shysta.sehgal@mail.utoronto.ca

"""

import pandas
import decoder
import pandas as pd

JP = 5
LS = 6


def get_curricula_vocabulary(file: str, month: int) -> pandas.DataFrame:
    """
    This function reads from a curriculum CSV file and returns the vocabulary that has been taught in that curriculum
    till that month.
    :param file: A CSV file that contains information from the coded curriculum
    :param month: An integer value from 1 or 2. If any other value, the vocabulary is returned for all the months. This
    decision was made to account for learning differences between JP and LS, as JP contains only two months worth of
    data.
    :return: A Pandas DataFrame with rows containing information about the month, phonology, orthography, et cetera of a
    word.
    """
    df = pd.read_csv(file, na_filter=False)

    # get only those columns and rows containing information needed for analysis
    df = df[["Week", "Type", "Lexicality", "Orthography", "Phonology"]]
    df = df.drop(df[(df.Lexicality == 'GPC')].index)
    df = df.drop(df[(df.Lexicality == 'Word sentence')].index)
    df = df.drop(df[(df.Lexicality == 'Word Sentence')].index)
    df = df.drop(df[(df.Lexicality == 'Word phrase')].index)
    df = df[df["Phonology"].str.contains("/") == False]  # if the word has two or more phonologies, drop that row
    df.reset_index(drop=True, inplace=True)  # reset index so that row numbers start from zero
    if month == 1:  # get relevant words for the first month
        ur_row = df.loc[(df['Week'] == 5) | (df['Week'] == '5')].index.tolist()
        df = df.iloc[:ur_row[0]]

    # get relevant words for the second month. This does not matter for JP, as it only contains two month data
    if month == 2 and file != "Jolly Phonics Curriculum Coding Sheet - Coder 1.csv":
        ur_row = df.loc[(df['Week'] == 9) | (df['Week'] == '9')].index.tolist()
        df = df.iloc[:ur_row[0]]
    df.reset_index(drop=True, inplace=True)  # reset index so that row numbers start from zero
    return df


def get_curricula_gpcs(file: str, month: int) -> dict[str, list[str]]:
    """
    This function returns the GPCs that have been taught in a curriculum up to a certain month. This function should be
    preferably used for getting GPCs from LS for 1 or 2 months and from JP for 1 month.
    :param file: A CSV file that contains information from the coded curriculum
    :param month: An integer value from 1 or 2. If any other value, GPCs are returned for all the months. This decision
    was made to account for learning differences between JP and LS, as JP contains only two months worth of
    data.
    :return: a dictionary with graphemes as keys and phoneme correspondences as a list as respective values
    """
    df = pd.read_csv(file, na_filter=False)

    # get only those columns and rows containing information needed for analysis
    df = df[["Week", "Lexicality", "Orthography", "Phonology"]]
    df = df.drop(df[(df.Lexicality == 'Word')].index)
    df = df.drop(df[(df.Lexicality == 'Word sentence')].index)
    df = df.drop(df[(df.Lexicality == 'Word Sentence')].index)
    df = df.drop(df[(df.Lexicality == 'Word phrase')].index)
    df.reset_index(drop=True, inplace=True)  # reset index so that row numbers start from zero
    if month == 1:  # get relevant GPCs for the first month
        ur_row = df.loc[(df['Week'] == 5) | (df['Week'] == '5')].index.tolist()
        df = df.iloc[:ur_row[0]]

    # get relevant words for the second month. This does not matter for JP, as it only contains two month data
    if month == 2 and file != "Jolly Phonics Curriculum Coding Sheet - Coder 1.csv":
        ur_row = df.loc[(df['Week'] == 9) | (df['Week'] == '9')].index.tolist()
        df = df.iloc[:ur_row[0]]
    df.reset_index(drop=True, inplace=True)  # reset index so that row numbers start from zero
    gpc_dic = df.groupby('Orthography')['Phonology'].apply(list).to_dict()  # create a dictionary

    # strip all whitespaces
    gpc_dic = {x.replace(' ', ''): v
               for x, v in gpc_dic.items()}
    for key in gpc_dic:
        for i in range(len(gpc_dic[key])):
            gpc_dic[key][i] = gpc_dic[key][i].strip()
    return gpc_dic


def get_transcription(df: pandas.DataFrame, phoneme_dic: dict[str, list[str]], greedy: str) -> tuple[float, int]:
    """
    This function returns the percentage of words correctly decoded from the dataframe depending on the rules taught in
    the dictionary. The user can choose different modes for reading of the words: big, small, and no as described in
    module decoder.py.
    :param df: A Pandas DataFrame with rows containing information about the month, phonology, orthography, et cetera of
    a word.
    :param phoneme_dic: a dictionary with graphemes as keys and phoneme correspondences as a list as respective values
    :param greedy: described in the decode_words function in decoder.py module
    :return: a tuple containing the percentage of correct words read, and the number of tricky words that could not
    decoded with the dictionary rules but were included in the correct words read (because these were taught to the
    students)
    """
    orthography = df["Orthography"].tolist()
    for i in range(len(orthography)):
        orthography[i] = orthography[i].lower()
        orthography[i] = orthography[i].strip()

    # get the transcriptions for the dataframe depending on the greedy mode
    transcriptions = decoder.decode_words(orthography, phoneme_dic, greedy)
    phonology = df["Phonology"].tolist()
    for i in range(len(phonology)):
        phonology[i] = phonology[i].strip()

    # to keep track of the explicit and implicit tricky words that could not be decoded from the rules in the dictionary
    type_list = df["Type"].tolist()
    count = 0  # words read correctly
    tricky_count = 0
    seen = []  # to keep track of pronunciations already accounted for in order to avoid duplicates
    if greedy == 'small' or greedy == 'big':
        for i in range(len(phonology)):
            if transcriptions[orthography[i]] == {}:
                if type_list[i] == "Tricky word":
                    count += 1
                    tricky_count += 1
                elif type_list[i] == "Exception-implicit":
                    count += 1
                    tricky_count += 1
            else:
                for key in transcriptions[orthography[i]]:
                    if phonology[i] not in seen:
                        seen.append(phonology[i])
                        if phonology[i] in transcriptions[orthography[i]][key]:
                            count += 1
                        elif type_list[i] == "Tricky word":
                            count += 1
                            tricky_count += 1
                        elif type_list[i] == "Exception-implicit":
                            count += 1
                            tricky_count += 1
    else:
        for i in range(len(phonology)):
            if phonology[i] not in seen:
                seen.append(phonology[i])
                if phonology[i] in transcriptions[orthography[i]]:
                    count += 1
                elif type_list[i] == "Tricky word":
                    count += 1
                    tricky_count += 1
                elif type_list[i] == "Exception-implicit":
                    count += 1
                    tricky_count += 1
    phonology = list(dict.fromkeys(phonology))  # to get rid of duplicates for percentage purposes
    return (count / len(phonology)) * 100, tricky_count


if __name__ == "__main__":
    # get the rules of the curricula you want to use. Use the function in the decoder module to get the rules for JP for
    # two months and more and for LS for only 2 months +. Specify alternative-implicit
    # otherwise, use the function in this module by specifying the month
    rules_dic = decoder.curricula_gpcs("phoneme dictionary.xlsx - Sheet1.csv", 5, False)

    # get the vocabulary for the curricula you want to use. Note the months for the rules and vocabulary must match
    curricula = get_curricula_vocabulary("Jolly Phonics Curriculum Coding Sheet - Coder 1.csv", 2)

    # uncomment the following line to see the output. Specify greedy mode.
    # print(get_transcription(curricula, rules_dic, 'no'))
