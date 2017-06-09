package main

import (
	"bufio"
	"os"
	"log"
	"strings"
	"sort"
	"sync"
)
type validWord struct {
	Word string
	Score int
	BranchCharacter string
}

type byScore []validWord

func (a byScore) Len() int           { return len(a) }
func (a byScore) Swap(i, j int)      { a[i], a[j] = a[j], a[i] }
func (a byScore) Less(i, j int) bool { return a[i].Score > a[j].Score }

func scrabble(branch string, hand string) []validWord {
	var wg sync.WaitGroup
	var validWords []validWord
	var freq = characterFrequency(hand)
        var blanks = freq['?']
	var word string

	for i := 0; i < len(branch); i++ {
		wg.Add(1)
		go func(branchChar byte) {
			defer wg.Done()
			var branchFreq map[byte]uint8
			branchFreq = characterFrequency(hand) // freq
			branchFreq[branchChar] = branchFreq[branchChar] + 1
			for j := 0; j < len(wordList); j++ {
				word = wordList[j]
				if (strings.Count(word, string(branchChar)) != 0 ) {
					if isValidWord(word, branchFreq, blanks) {
						validWords = append(validWords, validWord{word, wordScore(word, branchChar), string(branchChar)})
					}
				}
			}
		}(branch[i])
		wg.Wait()
	}
	sort.Sort(byScore(validWords))
	return validWords
}

func readFile(filename string) []string {
	wordList := make([]string, 83667)
	var i = int(0)
	file, err := os.Open(filename)
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		wordList[i] = scanner.Text()
		i+=1
	}

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
	return wordList
}

func characterFrequency(text string) map [byte]uint8 {
	var freq = make(map[byte]uint8)
	for i := 0; i < len(text); i++ {
		freq[text[i]] = freq[text[i]]+1
	}
	return freq
}

func isValidWord(word string,hand map [byte]uint8, blanks uint8) bool {
	var failed = uint8(0)
	for i := 0; i < len(word); i++ {
		if (hand[word[i]] < uint8(strings.Count(word, string(word[i])))){
			failed +=1
			if (failed > blanks) {
				return false
			}
		}
	}
	return true
}

func wordScore(word string, branch byte ) int {
	var score = int(0)
	for i := 0; i < len(word); i++ {
		score += letterValue[word[i]]
	}
	score -= letterValue[branch]
	return score
}