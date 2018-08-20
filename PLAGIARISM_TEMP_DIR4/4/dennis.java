public class wordCount
{
public static void main(String ar[]) throws Exception
{
System.out.println("Simple Java Word Count Program");


    int wordCount = 1,count=1;
 BufferedReader br = new BufferedReader(new FileReader("C:/file.txt"));
            String str2 = "", str1 = "";

            while ((str1 = br.readLine()) != null) {

                    str2 += str1;

            }


    for (int i = 0; i < str2.length(); i++) 
    {
        if (str2.charAt(i) == ' ' && str2.charAt(i+1)!=' ') 
        {
            wordCount++;
        } 


        }

    System.out.println("Word count is = " +(wordCount));
}
}