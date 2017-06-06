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

def isValidWord(word, hand, i):
    if i not in word: return False
    hand += i
    blanks = hand.count('?')
    failed = 0
    for i in word:
        if hand.count(i)<word.count(i):
          failed +=1
          if failed > blanks:
              return False
    return True

wordList = loadWords()
if __name__ == '__main__':
  while input("Any key to continue `q` to quit: ") != 'q':
    availableTiles = "".join(set(list(input("Enter the tiles already on the board: ")))).lower()
    hand = input("Enter the tiles that are in your hand: ").lower()
    start_time = timeit.default_timer()
    words = []
    for i in availableTiles:
      words.extend([(getWordScore(word, i),word, i) for word in wordList if isValidWord(word, hand,i)])
    print(sorted(set(words)))
    print("Time: ", timeit.default_timer() - start_time)
    
# A simple python script that chooses the best possible word to play in words with friends **Note** this does not take into consideration the state of the board only what the best words to play are
