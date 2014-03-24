from os import listdir
from random import shuffle


class Attrib:
    def __init__(self, name, pos, mood, sunglasses, eyes_closed, path, dim=None):
        self.name, self.pos, self.mood, self.sunglasses, self.eyes_closed, self.dim, self.path = name, pos, mood, sunglasses, eyes_closed, dim, path

    def __str__(self):
        return 'Name : ' + self.name + ', Pos : ' + self.pos + ', Mood : ' + self.mood + ', Sunglasses : ' + str(self.sunglasses) + ', Eyes closed : ' + str(self.eyes_closed) + ', Dim : ' + str(self.dim) + ', Path : ' + self.path

    def __unicode__(self):
        return self.__str__()

    def __repr__(self):
        return self.__str__()

    def __eq__(self, other):
        return self.path == other.path


def listdir_nohidden(path):
    output = []
    for f in listdir(path):
        if not f.startswith('.'):
            output.append(f)
    return output


def analyze_image(filepath):
    filename = filepath[1+filepath.rfind('/'):]
    words = filename.split('_')
    words[-1] = words[-1][0:words[-1].find('.')]
    return Attrib(words[0], words[1], words[2], words[3] == 'sunglasses', words[3] == 'closed', filepath, '' if len(words) < 5 else words[4])


def load_image(dir):
    output = []
    for d in dir:
        i = 0
        for f in listdir_nohidden(d):
            for ff in listdir_nohidden(d+f):
                output.append(analyze_image(d + f + '/' + ff))
                i += 1
        print(d, ' contains ', i, ' items')
    shuffle(output)
    return output


def query(dirs, n, name=None, pos=None, mood=None, sunglasses=None, eyes_closed=None, dim='', different_than=None):
    output = []
    i = 0
    params = {'name':name, 'pos':pos, 'mood':mood, 'sunglasses':sunglasses, 'eyes_closed':eyes_closed, 'dim':dim}
    for f in load_image(dirs):
        if i >= n:
            break

        flag = True
        for k, v in params.items():
            if v is not None:
                vv = getattr(f, k)
                flag = v == vv
            if not flag:
                break
        if flag:
            if different_than is None or (different_than is not None and f not in different_than):
                output.append(f)
                i += 1
    return output


def save_file(path, list):
    with open(path, 'w+') as f:
        for l in list:
            f.write(l.path + '\n')

# The data is from the Machine Learning book
d1 = '/Users/diego/Dropbox/_Alexandre/TI/face-1_sunglasses/afs/cs/project/theo-8/faceimages/faces/' # -> sunglasses are open or sunglasses (in filepath)
d2 = '/Users/diego/Dropbox/_Alexandre/TI/faces-2_eyes_opened_closed/' # -> eyes are open or closed (in filepath)

training = query([d1, d2], 10, name=None, pos=None, mood=None, sunglasses=None, eyes_closed=None, dim='4', different_than=None)
validation = query([d1, d2], 10, name=None, pos=None, mood=None, sunglasses=None, eyes_closed=None, dim='4', different_than=training)

print('\nTraining ', len(training), ' items\n')
for t in training:
    print(t)
print('\nValidation', len(validation), ' items\n')
for v in validation:
    print(v)

save_file('/Users/diego/Desktop/training.list', training)
save_file('/Users/diego/Desktop/validation.list', validation)