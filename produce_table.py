import pandas as pd
from sklearn import preprocessing


def read_file(file: str) -> str:
    with open(file) as f:
        line = f.readline()
        i = 1
        arr_0 = line.split(",")
        length = len(arr_0)
        while line != '':
            arr_1 = line.split(",")
            if len(arr_1) != length:
                return "Problem with data"
            i += 1
            line = f.readline()
    return "No problems with prelim analysis"


def get_data(file: str):
    with open(file) as f:
        line = f.readline()
        arr_0 = line.split(",")
        arr_0.remove(arr_0[0])
        for i in range(len(arr_0)):
            arr_0[i] = arr_0[i].strip()
    return arr_0


def get_features(file: str):
    with open(file) as f:
        f.readline()
        line = f.readline()
        temp = []
        while line != '':
            feature = line.split(",")
            for i in range(len(feature)):
                feature[i] = feature[i].strip()
            temp.append(feature)
            line = f.readline()
        return temp


def get_vector(feature_list: list, phoneme: list):
    dic = {}
    for i in range(len(phoneme)):
        dic[phoneme[i]] = []
        for j in range(len(feature_list)):
            dic[phoneme[i]].append(feature_list[j][i + 1])
    return dic


def get_overlap(dic: dict):
    lis = []
    lis3 = []
    temp = []
    for key in dic:
        for key2 in dic:
            if key2 != key and [key, key2] not in temp and [key2, key] not in temp:
                j = 0
                for i in range(len(dic[key])):
                    if dic[key][i] == dic[key2][i]:
                        j += 1
                lis.append([key, key2])
                lis3.append(j)
                temp.append([key, key2])
                temp.append([key2, key])
    return [lis, lis3]


if __name__ == "__main__":
    lis1 = get_features("CH68.txt")
    lis2 = get_data("CH68.txt")
    dictionary = get_vector(lis1, lis2)
    arr = get_overlap(dictionary)
    df = pd.DataFrame(list(zip(arr[0], arr[1])), columns=['phonemes', 'overlap'])
    s_array = df['overlap']
    normalized = preprocessing.normalize([s_array])
    new_arr = normalized[0]
    df['overlap'] = new_arr
    df.to_csv("/Users/shystasehgal/Desktop/SiR/Analysis/output.csv")
