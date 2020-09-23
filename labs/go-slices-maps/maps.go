package main

import (
	"golang.org/x/tour/wc"
	"strings"
)

func WordCount(s string) map[string]int {
	m:=make(map[string]int)
	words := strings.SplitAfter(s," ")
	for i:=0;i<len(words);i++{
		m[strings.Trim(words[i]," ")]+=1
	}
	return m
}

func main() {
	wc.Test(WordCount)
}
