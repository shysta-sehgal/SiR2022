import produce_table
import pandas as pd
from sklearn import preprocessing

lis1 = produce_table.get_features("CH68.txt")
lis2 = produce_table.get_data("CH68.txt")
dictionary = produce_table.get_vector(lis1, lis2)
arr = produce_table.get_overlap(dictionary)
df_1 = pd.DataFrame(list(zip(arr[0], arr[1])), columns=['phonemes', 'overlap'])
s_array = df_1['overlap'].to_numpy()
normalized = preprocessing.normalize([s_array])
new_arr = normalized[0]
phoneme_arr = list(df_1['phonemes'])


def initial_processing(phoneme: list):
    arr_1 = []
    arr_2 = []
    for j in range(len(phoneme)):
        arr_1.append(phoneme[j][0])
        arr_2.append(phoneme[j][1])
    return arr_1, arr_2


if __name__ == '__main__':
    tup = initial_processing(phoneme_arr)
    phoneme_1 = tup[0].copy()
    phoneme_2 = tup[1].copy()
    phoneme_1 = list(dict.fromkeys(phoneme_1))
    phoneme_2 = list(dict.fromkeys(phoneme_2))
    df = pd.DataFrame(list(phoneme_1), columns=['phonemes'])
    df = df.reindex(columns=df.columns.tolist() + phoneme_2)
    x = df['phonemes'][0]
    print(x)
