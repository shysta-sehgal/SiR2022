"""
This module decodes words into their respective IPA transcriptions.
There are three strategies to do this:
1. Exhaustive strategy: gets all possible IPA transcriptions for a word
2. Greedy small: gets only those IPA transcriptions for a word that can be formed by combining the maximum number of
GPCs
3. Greedy big: gets only those IPA transcriptions for a word that can be formed by combining the minimum number of GPCs

The module can do so for specific curriculum and also a general set of GPCs. For specific curriculum, this module has
used Jolly Phonics and Letters and Sounds. In these curricula, some GPCs have been taught implicitly, and the user can
specify whether they want these GPCs to be used by the decoder or not.

Precondition: The file used is a CSV file.
The file must be formatted in the following manner:
<A header row>
<grapheme1, phoneme1, x, x, x, x, curricula_1 GPC (1 or 0), curricula_2 GPC (2, 1 or 0)>
<grapheme2, phoneme2, x, x, x, x, curricula_1 GPC (1 or 0), curricula_2 GPC (2, 1 or 0)>
...
...

x indicates the values that have not been used for analysis in this module and could be potentially anything. The
curricula specific columns indicate whether the grapheme for that row is present in that curriculum or not, where 2
represents GPCs that have been implicitly taught, 1 represents GPCs that have been taught, and 0 represents GPCs that
have not been taught.
The user can potentially use a file without these specific columns if they do not want to use this functionality. The
user can also additional curriculum-specific columns after these columns if they want to analyse some other curricula.
The global variables might need to be changed or declared according to the user's needs.
"""

from typing import Union

JP = 5  # column position of jolly phonics GPCs in the CSV file
LS = 6  # column position of jolly phonics GPCs in the CSV file


def read_orthography(file: str) -> dict[str, list[str]]:
    """
    This function reads through a CSV file and returns a dictionary of graphemes with their corresponding phonemes.
    :param file: A CSV file that contains information about graphemes and their corresponding phonemes
    :return: A dictionary that contains the graphemes as the keys and a list of corresponding phonemes as the values
    """
    with open(file) as f:
        f.readline()  # skip the first line, as it only contains headers
        line = f.readline()
        phoneme_dic = {}
        while line != '':
            if line.split(',')[0] not in phoneme_dic:
                phoneme_dic[line.split(',')[0]] = [line.split(',')[1]]
            else:
                phoneme_dic[line.split(',')[0]].append(line.split(',')[1])
            line = f.readline()
        return phoneme_dic


def get_substrings(word: str, phoneme_dict: dict[str, list[str]]) -> list[str]:
    """
    This is a helper function for the recursive decoding method. This function returns a list of all substrings of a
    string such that the substrings do not contain the first letter of the string. Moreover, these substrings also have
    phoneme correspondences in the dictionary <phoneme_dict>.
    :param word: the word whose substrings need to be generated
    :param phoneme_dict: a dictionary containing grapheme-phoneme correspondences
    :return: a list of all substrings of the string, excluding those substrings that contain the first letter of the
    string such that these substrings have a phoneme correspondence in the dictionary
    """
    res = [word[i: j] for i in range(1, len(word))  # get all substrings except first letter substrings
           for j in range(i + 1, len(word) + 1)]
    res_cpy = []

    # append only those items to the list that have phoneme correspondences
    while res != []:
        for item in res:
            if item not in phoneme_dict:
                res.remove(item)
            else:
                res_cpy.append(item)
                res.remove(item)
    return res_cpy


def get_first_substring(word: str, phoneme_dict: dict[str, list[str]]) -> list[str]:
    """
    This is a helper function for the recursive decoding function. It returns all substrings of the the word such that
    they have a corresponding IPA transcription and contain the first letter of the string.
    :param word: the word whose substrings are to be returned
    :param phoneme_dict: a dictionary containing grapheme-phoneme correspondences
    :return: a list of all substrings of the string, such that those substrings contain the first letter of the string
    and these substrings have a phoneme correspondence in the dictionary
    """
    first_letter_strings = []
    for i in range(len(word)):
        if word[:i + 1] in phoneme_dict:
            first_letter_strings.append(word[:i + 1])  # list with first letter substrings with ipa transcription
    return first_letter_strings


def decode_words(words: list[str], phoneme_dict: dict[str, list[str]], greedy: str) -> \
        Union[dict[str, list[str]], dict[str, dict[int, list[str]]]]:
    """
    This function takes a list of target words and decodes them into all possible IPA transcriptions depending on the
    greedy mode chosen.
    :param words: List of words to be decoded into IPA
    :param phoneme_dict: a dictionary containing grapheme-phoneme correspondences
    :param greedy: Must take values only from 'small', 'big', or 'no'. If 'no' is selected, an exhaustive list of IPA
    transcriptions is returned. If 'big' is chosen, only those transcriptions that required the minimum recursions is
    returned (the logic follows from mapping the largest graphemes first). If 'small' is chosen, only those
    transcriptions that required the maximum recursions is returned (the logic follows from mapping the smallest
    graphemes first).
    :return: If greedy modes 'small' or 'big' are chosen, a dictionary containing target words as keys and a dictionary
    as values containing the number of recursions made for those words as keys and list of transcriptions as values is
    returned. If 'no' is chosen, a dictionary containing target words as keys and a list of transcriptions as values is
    chosen.
    """
    decoder = {}
    for word in words:
        word = word.lower()
        word = word.strip()

        # if no grapheme-phoneme correspondences exist
        if len(word) < 1 and greedy == 'small' or greedy == 'big':
            decoder[word] = {}
        elif len(word) < 1:
            decoder[word] = []
        if len(word) == 1 and word not in phoneme_dict and greedy == 'small' or greedy == 'big':
            decoder[word] = {}
        elif len(word) == 1 and word not in phoneme_dict:
            decoder[word] = []

        # get substrings except the first letter substrings from helper
        substrings = get_substrings(word, phoneme_dict)

        # get substrings containing the first letter
        first_letter_strings = get_first_substring(word, phoneme_dict)

        if first_letter_strings == [] and greedy == 'small':
            decoder[word] = {}
        elif first_letter_strings == [] and greedy == 'big':
            decoder[word] = {}
        elif first_letter_strings == []:
            decoder[word] = []

        # get possible IPA transcriptions of the word building upon each first letter substring for the word
        for ss in first_letter_strings:
            path = (phoneme_dict[ss])
            master = get_transcriptions(ss, substrings, word, phoneme_dict, path, 1)
            if greedy == 'small':
                greedy_small(master, decoder, word)
            elif greedy == 'big':
                greedy_big(master, decoder, word)
            else:
                exhaustive(master, decoder, word)
    return decoder


def get_transcriptions(first_letter_ss: str, substrings: list[str], word: str, phoneme_dic: dict[str, list[str]],
                       path: list[str], counter: int) -> dict[int: list[str]]:
    """
    This function is a recursive helper function for <decode_words>. It returns a dictionary with the number of
    recursions performed to get particular IPA transcriptions as keys and a list of IPA transcriptions as values. It
    gets these IPA transcriptions based on the first letter substring being passed.
    :param first_letter_ss: a particular substring of the word containing the first letter
    :param substrings: a list of all substrings of the word containing IPA transcriptions except the first
    letter substrings
    :param word: word to be decoded
    :param phoneme_dic: a dictionary containing grapheme-phoneme correspondences
    :param path: the current path of the decoded word, i.e., the current IPA transcription of the word, which may be
    partial or incomplete
    :param counter: the current number of recursions done. The number of recursions start at 1 and not 0.
    :return:  a dictionary with the number of recursions performed to get particular IPA transcriptions as keys and a
    list of IPA transcriptions as values
    """

    # base case 1 (when the first letter substring is already equal to the word)
    if first_letter_ss == word:
        return {counter: path}

    # base case 2 ( when the length of the first letter substring is more than or equal to the word but the strings do
    # not match
    elif len(first_letter_ss) >= len(word):
        return {counter: []}

    transcriptions = {}

    # go through the substrings list to check which combinations can make up the word
    for substring in substrings:
        if first_letter_ss + substring in word:
            partial_word = first_letter_ss + substring
            counter_cpy = counter  # do this step to prevent aliasing
            counter_cpy += 1  # increase the number of recursions done as this partial_word is in word
            results_cpy = substrings.copy()  # do this step to prevent aliasing

            # remove the substring from the list to avoid checking it again in further recursions
            results_cpy.remove(substring)
            path_cpy = []

            # for all partial transcriptions currently in the path, add all possible combinations of the IPA
            # transcriptions of the substring being checked
            for i in range(len(path)):
                for ipa in phoneme_dic[substring]:
                    path_cpy.append(path[i] + ipa)

            # recurse to get all possible IPA transcriptions building upon partial_word
            transcription = get_transcriptions(partial_word, results_cpy, word, phoneme_dic, path_cpy, counter_cpy)
            for key in transcription:
                if key not in transcriptions:
                    transcriptions[key] = transcription[key]
                else:
                    for value in transcription[key]:
                        transcriptions[key].append(value)
    return transcriptions


def greedy_small(master: dict[int: list[str]], decoder: dict[str, dict[int, list[str]]], word: str) -> None:
    """
    This function takes the decoder dictionary and modifies it such that the value dictionary for the word in <decoder>
    contains only those IPA transcriptions with the maximum recursions.
    :param master: a dictionary with the number of recursions performed to get particular IPA transcriptions as keys and
    a list of IPA transcriptions as values
    :param decoder: a dictionary containing target words as keys and a dictionary as values containing the number of
    recursions made for those words as keys and list of transcriptions as values
    :param word: the word that was decoded
    :return: None
    """
    maxi = -1
    if master != {}:

        # get the key in the dictionary that represents the maximum recursions
        for key in master:
            if key >= maxi:
                maxi = key

    # the word was not decodable for a particular instance of the first letter substring
    elif word not in decoder or (word in decoder and decoder[word] == {}):
        decoder[word] = {}
        return

    # master is not empty and word is present in decoder
    if word in decoder and decoder[word] != {}:
        for key in decoder[word]:
            if key > maxi:  # already a key with more recursions present
                pass
            elif key == maxi:  # key with same number of recursions present, append to it
                for value in master[maxi]:
                    if value not in decoder[word][key]:  # to avoid duplicates
                        decoder[word][key].append(value)
            else:  # we have the key with maximum recursions
                decoder[word] = {}
                decoder[word][maxi] = []
                for value in master[maxi]:
                    if value not in decoder[word][maxi]:  # to avoid duplicates
                        decoder[word][maxi].append(value)
    else:  # word was not in decoder or decoder[word] was empty
        decoder[word] = {}
        decoder[word][maxi] = []
        for value in master[maxi]:
            if value not in decoder[word][maxi]:
                decoder[word][maxi].append(value)


def greedy_big(master: dict[int: list[str]], decoder: dict[str, dict[int, list[str]]], word: str) -> None:
    """
    This function takes the decoder dictionary and modifies it such that the value dictionary for the word in <decoder>
    contains only those IPA transcriptions with the minimum recursions.
    :param master: a dictionary with the number of recursions performed to get particular IPA transcriptions as keys and
    a list of IPA transcriptions as values
    :param decoder: a dictionary containing target words as keys and a dictionary as values containing the number of
    recursions made for those words as keys and list of transcriptions as values
    :param word: the word that was decoded
    :return: None
    """
    maxi = len(word)
    if master != {}:

        # get the key in the dictionary that represents the minimum recursions
        for key in master:
            if key <= maxi:
                maxi = key

    # the word was not decodable for a particular instance of the first letter substring
    elif word not in decoder or (word in decoder and decoder[word] == {}):
        decoder[word] = {}
        return

    # master is not empty and word is present in decoder
    if word in decoder and decoder[word] != {}:
        for key in decoder[word]:
            if key > maxi:  # we have the key with minimum recursions
                decoder[word] = {}
                decoder[word][maxi] = []
                for value in master[maxi]:
                    if value not in decoder[word][maxi]:  # to avoid duplicates
                        decoder[word][maxi].append(value)
            elif key == maxi:  # key with same number of recursions present, append to it
                for value in master[maxi]:
                    if value not in decoder[word][key]:  # to avoid duplicates
                        decoder[word][key].append(value)
    else:  # word was not in decoder or decoder[word] was empty
        decoder[word] = {}
        decoder[word][maxi] = []
        for value in master[maxi]:
            if value not in decoder[word][maxi]:  # to avoid duplicates
                decoder[word][maxi].append(value)


def exhaustive(master: dict[int: list[str]], decoder: dict[str, list[str]], word: str) -> None:
    """
    This function takes all the transcriptions for the word that are present in <master> and adds it to the decoder
    dictionary
    :param master: a dictionary with the number of recursions performed to get particular IPA transcriptions as keys and
    a list of IPA transcriptions as values
    :param decoder: A dictionary with words as keys and a list of IPA transcriptions as their values
    :param word: the word that was decoded
    :return: None
    """
    # there were no IPA transcriptions for a particular instance of the first letter substring
    if master == {0: []} and word not in decoder:
        decoder[word] = []

    # if the word was already in the decoder for some transcriptions but master was empty
    elif master == {0: []}:
        pass
    else:  # master is not empty and word might or might not be present in the decoder
        if word not in decoder:
            decoder[word] = []

        # add all transcriptions from master to the decoder dictionary, without duplicates
        for item in master:
            for item2 in master[item]:
                if item2 not in decoder[word]:
                    decoder[word].append(item2)


def curricula_gpcs(file: str, curricula: int, alternative_implicit: bool) -> \
        dict[str, list[str]]:
    """
    This function returns the graphemes as keys and phoneme correspondences as a list as respective values that have
    been taught in either Jolly Phonics or Letters and Sounds curricula.
    :param file: A CSV file that contains information about graphemes and their corresponding phonemes
    :param curricula: 5 for Jolly Phonics and 6 for Letters and Sounds
    :param alternative_implicit: tells if we need to account for implicitly taught GPCs in each curricula
    :return: a dictionary with graphemes as keys and phoneme correspondences as a list as respective values
    """
    file_handle = open(file)
    read = file_handle.read()
    lines = read.split('\n')  # split the file by the rows
    rows = []
    for comma in lines:  # split each individual line by commas
        rows.append(comma.split(','))
    gpcs = {}
    for i in rows:
        if alternative_implicit:  # if we want GPCs that have been implicitly taught in each curricula
            if i[curricula].strip() == '1' or i[curricula].strip() == '2':
                if i[0] not in gpcs:
                    gpcs[i[0]] = [i[1]]
                else:
                    gpcs[i[0]].append(i[1])
        else:  # if we do not want GPCs that have been implicitly taught in each curricula
            if i[curricula].strip() == '1':
                if i[0] not in gpcs:
                    gpcs[i[0]] = [i[1]]
                else:
                    gpcs[i[0]].append(i[1])
    return gpcs


if __name__ == '__main__':
    phonemes = read_orthography("phoneme dictionary.xlsx - Sheet1.csv")  # get all GPCs from the file

    # get curriculum specific (Jolly Phonics) phonemes. Change JP to LS for Letters and Sound, with implicit GPCs
    curr_phonemes = curricula_gpcs("phoneme dictionary.xlsx - Sheet1.csv", JP, True)

    # get exhaustive IPA transcriptions for all GPCs. Change 'no' to 'small' or 'big' for other modes.
    all_ipa = decode_words(['set', ' ', 'palace', 'war', 'ok', 'set', 'beauty', 'star', 'hear'],
                           phonemes, 'big')

    # get exhaustive IPA transcriptions for jolly phonics GPCs. Change 'no' to 'small' or 'big' for other modes.
    jp_ipa = decode_words(['set', 'ten', 'joker', 'good', 'there'], curr_phonemes, 'big')

    # uncomment the next couple of lines to see the output
    # print(all_ipa)
    # print(jp_ipa)
