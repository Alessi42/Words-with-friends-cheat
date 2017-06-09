package main

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"fmt"
	"strings"
)

var DB = make(map[string]string)
var wordList = readFile("words.csv")
var letterValue = map[byte]int{
	'A': 1, 'B': 4, 'C': 4, 'D': 2, 'E': 1, 'F': 4, 'G': 3, 'H': 3, 'I': 1, 'J': 10, 'K': 5, 'L': 2, 'M': 4, 'N': 2, 'O': 1, 'P': 4, 'Q': 10, 'R': 1, 'S': 1, 'T': 1, 'U': 2, 'V': 5, 'W': 4, 'X': 8, 'Y': 3, 'Z': 10,
}

func main() {
	DB["jacob"] = "ok"
	gin.SetMode(gin.DebugMode)
	r := gin.Default()

	// Ping test
	r.GET("/ping", func(c *gin.Context) {

		c.String(200, "pong")
	})
	r.GET("/scrabble/:branch/:hand", func(c *gin.Context) {
		var branch string
		var hand string
		var validWords []validWord
		branch = strings.ToUpper(string(c.Param("branch")))

		hand = strings.ToUpper(string(c.Param("hand")))

		validWords = scrabble(branch,hand)
		numValid := len(validWords)
		if numValid > 32 {
			numValid = 32
		}
		c.JSON(http.StatusOK, gin.H{"branch": branch ,"hand": hand,  "validWords": validWords[0:numValid]})
	})
		// Get user value
	r.GET("/user/:name", func(c *gin.Context) {
		user := c.Params.ByName("name")
		value, ok := DB[user]
		if ok {
			c.JSON(http.StatusOK, gin.H{"user": user, "value": value})
		} else {
			fmt.Print("error inbound")
			var freq = characterFrequency(wordList[4])
			fmt.Print(freq)
			c.JSON(http.StatusOK, gin.H{"user": wordList[4], "status": freq})
		}
	})

	// Authorized group (uses gin.BasicAuth() middleware)
	// Same than:
	// authorized := r.Group("/")
	// authorized.Use(gin.BasicAuth(gin.Credentials{
	//	  "foo":  "bar",
	//	  "manu": "123",
	//}))
	authorized := r.Group("/", gin.BasicAuth(gin.Accounts{
		"foo":  "bar", // user:foo password:bar
		"manu": "123", // user:manu password:123
	}))

	authorized.POST("admin", func(c *gin.Context) {
		user := c.MustGet(gin.AuthUserKey).(string)

		// Parse JSON
		var json struct {
			Value string `json:"value" binding:"required"`
		}

		if c.Bind(&json) == nil {
			DB[user] = json.Value
			c.JSON(200, gin.H{"status": "ok"})
		}
	})

	// Listen and Server in 0.0.0.0:8080
	r.Run(":8080")
}
