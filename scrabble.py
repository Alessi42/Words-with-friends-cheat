import timeit
WORDLIST_FILENAME = "words.txt"
SCRABBLE_LETTER_VALUES = {
    'a': 1, 'b': 4, 'c': 4, 'd': 2, 'e': 1, 'f': 4, 'g': 3, 'h': 3, 'i': 1, 'j': 10, 'k': 5, 'l': 2, 'm': 4, 'n': 2, 'o': 1, 'p': 4, 'q': 10, 'r': 1, 's': 1, 't': 1, 'u': 2, 'v': 5, 'w': 4, 'x': 8, 'y': 3, 'z': 10
}

def getWordScore(word,i):
    return sum([SCRABBLE_LETTER_VALUES[y] for y in word]) - SCRABBLE_LETTER_VALUES[i]
  
def loadWords():
    inFile = open(WORDLIST_FILENAME, 'r')
    wordList = []
    for line in inFile:
        wordList.append(line.strip().lower())
    return wordList

def getFrequencyDict(sequence):
    """
    Returns a dictionary where the keys are elements of the sequence
    and the values are integer counts, for the number of times that
    an element is repeated in the sequence.

    sequence: string or list
    return: dictionary
    """
    # freqs: dictionary (element_type -> int)
    freq = {}
    for x in sequence:
        freq[x] = freq.get(x,0) + 1
    return freq

def isValidWord(word, hand, i):
    if i not in word: return False
    blanks = 0 if hand.get('?') == None else hand.get('?')
    failed = 0
    for i in word:
        if hand.get(i)==None or hand.get(i)<word.count(i):
          failed +=1
          if failed > blanks:
              return False
    return True

wordList = loadWords()
#wordList = ['agonize', 'ark', 'asunder', 'boskier', 'butlery', 'cheekier', 'clop', 'cymlins', 'deep', 'divest', 'erlking', 'faubourg', 'finery', 'fjelds', 'fowled', 'gallic', 'gangers', 'grainer', 'gullied', 'hornist', 'ideally', 'jiggly', 'loam', 'loathe', 'marooned', 'mhos', 'mistrial', 'mix', 'nitchies', 'nitrile', 'noblesse', 'olden', 'pygmaean', 'rearmost', 'residues', 'roadways', 'schemes', 'scotoma', 'seawan', 'slanged', 'sooners', 'splasher', 'stooked', 'stuffy', 'transact', 'treaty', 'ukes', 'venally', 'whiffers', 'xebecs']
while input("Any key to continue `q` to quit: ") != 'q':
    availableTiles = set("".join(sorted(set(list(input("Enter the tiles already on the board: "))))).lower())
    # branch
    hand = sorted(input("Enter the tiles that are in your hand: ").lower())
    #availableTiles =  sorted(set('asdafdahsrdfbghthrewsdfsdsddsuutoyubdd'.lower()))
    #hand = sorted('sdgthqereyriutycszxvbnu'.lower())
    start_time = timeit.default_timer()
    words = []
    alphawords = ''
    for i in availableTiles:
      fullHand = getFrequencyDict(str(hand)+str(i))
      iwords = [(getWordScore(word, i),word, i) for word in wordList if isValidWord(word, fullHand,i)]
      words.extend(iwords)
      #alphawords += "\n"+str(i)+str(sorted(iwords, reverse = True)[-50:])+'\n'
    #print(alphawords)
    print(sorted(words))
    print("Time: ", timeit.default_timer() - start_time)
    
## TODO make this a golang backend
