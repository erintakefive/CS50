# Made the body code of this one all myself. The template was
# however already provided by CS50 staff.

import nltk

from nltk.tokenize import TweetTokenizer

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        # make lists
        self.plist = []
        self.nlist = []
        
        # for each useful word in "positives" file, append word to plist
        with open(positives, "r") as pf:
            for line in pf:
                if line.startswith(";") is False and line.startswith("\n") is False:
                    self.plist.append(line.strip("\n"))
        
        pf.close()
        
        # repeat process for "negatives" file
        with open(negatives, "r") as nf:
            for line in nf:
                if line.startswith(";") is False and line.startswith("\n") is False:
                    self.nlist.append(line.strip("\n"))
        
        nf.close()
        
    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        # make variables
        sentiment = 0.0
        
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        
        # analyse text for sentiments
        for item in tokens:
            if item.lower() in self.plist:
                sentiment += 1
            elif item.lower() in self.nlist:
                sentiment -= 1

        return sentiment
