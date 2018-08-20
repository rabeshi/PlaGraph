
/*
   This program will read words from an input file, and count the
   number of occurrences of each word.  The word data is written to
   an output file twice, once with the words in alphabetical order
   and once with the words ordered by number of occurrences.
   The input and output file names must be given as command line
   arguments when the program is run.  The file uses the non-standard
   classes defined in the file TextReader.java.
   
   The program demonstrates several parts of Java's framework for
   generic programming:  TreeMap, List sorting, Comparators, etc.
*/

import java.io.*;
import java.util.*;

public class WordCount {

   static TextReader in;   // An input stream for reading the input file.
   static PrintWriter out; // Output stream for writing the output file.
   
   static class WordData { 
         // Represents the data we need about a word:  the word and
         // the number of times it has been encountered.
      String word;
      int count;
      WordData(String w) {
            // Constructor for creating a WordData object when
            // we encounter a new word.
         word = w;
         count = 1;  // The initial value of count is 1.
      }
   } // end class WordData
   
   static class CountCompare implements Comparator {
         // A comparator for comparing objects of type WordData
         // according to their counts.  This is used for
         // sorting the list of words by frequency.
      public int compare(Object obj1, Object obj2) {
         WordData data1 = (WordData)obj1;
         WordData data2 = (WordData)obj2;
         return data2.count - data1.count;
            // The return value is positive if data2.count > data1.count.
            // I.E., data1 comes after data2 in the ordering if there
            // were more occurrences of data2.word than of data1.word.
            // The words are sorted according to decreasing counts.
      }
   } // end class CountCompare


   public static void main(String[] args){
         // The program opens the input and output files.  It reads
         // words from the input file into a TreeMap, in which 
         // they are sorted by alphabetical order.  The words
         // are copied into a List, where they are sorted according
         // to frequency.  Then the words are copied from the
         // data structures to the output file.
   
      openFiles(args);  // Opens input and output streams, using file
                        // names given as command-line parameters.
      
      TreeMap words;  // TreeMap in which keys are words and values
                      // are objects of type WordData.
      words = new TreeMap(); 
      
      readWords(in,words);  // Reads words from the input stream and
                            // stores data about them in words.
   
      List wordsByCount;  // List will contain all the WordData
                          // values form the TreeMap, and will be
                          // sorted according to frequency count.
      wordsByCount = new ArrayList(words.values());
      Collections.sort(wordsByCount, new CountCompare());

      out.println("Words found in the file named \"" + args[0] + "\".\n");
      out.println("The number of times that the word occurred in the");
      out.println("file is given in parentheses after the word.\n\n");
      out.println("The words from the file in alphabetical order:\n");
      
      printWords(out, words.values());  // Print words alphabetically.

      out.println("\n\nThe words in order of frerquency:\n");
      
      printWords(out, wordsByCount);  // Prints words by frequency count.
      
      if (out.checkError()) {
            // Some error occurred on the output stream.
         System.out.println("An error occurred while writing the data.");
         System.out.println("Output file might be missing or incomplete.");
         System.exit(1);
      }
      
      System.out.println(words.size() + " distinct words were found.");

   } // end main()
   
   
   static void printWords(PrintWriter outStream, Collection wordData) {
         // wordData must contain objects of type WordData.  The words
         // and counts in these objects are written to the output stream.
      Iterator iter = wordData.iterator();
      while (iter.hasNext()) {
         WordData data = (WordData)iter.next();
         outStream.println("   " + data.word + " (" + data.count + ")");
      }
   } // end printWords()
   

   static void openFiles(String[] args) {
          // Open the global variable "in" as an input file with name args[0].
          // Open the global variable "out" as an output file with name args[1].
          // If args.length != 2, or if an error occurs while trying to open
          // the files, then an error message is printed and the program
          // will be terminated.
      if (args.length != 2) {
         System.out.println("Error: Please specify file names on command line.");
         System.exit(1);
      }
      try {
         in = new TextReader(new FileReader(args[0]));
      }
      catch (IOException e) {
         System.out.println("Error: Can't open input file " + args[0]);
         System.exit(1);
      }
      try {
         out = new PrintWriter(new FileWriter(args[1]));
      }
      catch (IOException e) {
         System.out.println("Error: Can't open output file " + args[1]);
         System.exit(1);
      }
   } // end openFiles()
   

   static void readWords(TextReader inStream, Map words) {
        // Read all words from inStream, and store data about them in words.
        // A word is any sequence of letters.  Words are converted to lower
        // case.  Any non-letters in the input stream are ignored.
        // When a word is encountered for the first time a key/value pair
        // is inserted into words.  The key is the word and the value is
        // a new object of type WordData containing the word.  When a word
        // is encountered again, the frequency count in the corresponding
        // WordData object is just increased by one.  If an error occurs
        // while trying to read the data, an error message is printed and
        // the program is terminated.

      try {
         while (true) { 
            while (! inStream.eof() && ! Character.isLetter(inStream.peek()))
               inStream.getAnyChar();  // Skip past non-letters.
            if (inStream.eof())
               break;  // Exit because there is no more data to read.
            String word = inStream.getAlpha();  // Read one word from stream.
            word = word.toLowerCase();
            WordData data = (WordData)words.get(word);
                // Check whether the word is already in the Map.  If not,
                // the value of data will be null.  If it is not null, then
                // it is a WordData object containing the word and the
                // number of times we have encountered it so far.
            if (data == null) {
                  // We have not encountered word before.  Add it to
                  // the map.  The initial frequency count is
                  // automatically set to 1 by the WordData constructor.
               words.put(word, new WordData(word) );
            }
            else {
                  // The word has already been encountered, and data is
                  // the WordData object that holds data about the word.
                  // Add 1 to the frequency count in the WordData object.
               data.count = data.count + 1;
            }
         }
      }
      catch (TextReader.Error e) {
         System.out.println("An error occurred while reading the data.");
         System.out.println(e.toString());
         System.exit(1);
      }

   } // end readWords()
   
   
} // end class WordCount