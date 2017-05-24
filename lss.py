import os
import re
import sys
from itertools import groupby
from operator import itemgetter

regex = '^(.*)((?<![a-zA-Z0-9])\d+)(.*)$'


def main(path):
    shots = {}
    extras = []

    for f in os.listdir(path):
        match = re.match(regex, f)
        if match:
            shot, frame, ext = match.groups()

            if shot not in shots:
                shots[shot] = {'frames': [], 'ext': '', 'padding': 0}

            shots[shot]['frames'].append(int(frame))
            shots[shot]['ext'] = ext
            shots[shot]['padding'] = len(frame)

        else:
            extras.append(f)

    for name, value in shots.iteritems():
        for _, group in groupby(enumerate(sorted(value['frames'])), lambda (i, x): i - x):
            frame_range = map(itemgetter(1), group)

            print "{0}%0{1}d{2}\t\t\033[92m({3})\t{4}-{5}\033[0m".format(name, value['padding'], value['ext'],
                                                                         len(frame_range), min(frame_range),
                                                                         max(frame_range)).expandtabs(10)

    print "\n".join(extras)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        main(sys.argv[1])
    else:
        curr_dir = os.path.abspath('.')
        main(curr_dir)
