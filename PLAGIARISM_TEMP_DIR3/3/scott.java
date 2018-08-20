
public class Dog {
	String name;
	int age;

	public String getName(){
		return name;
	}

	public void setName(String n){
		name = n;
	}

	public int getAge(){
		return age;
	}

	public void setAge(int n){
		age = n;
	}

	public void bark(){
		System.out.println(name + ":woof");
	}

	public Dog(String n,int no){
		name = n;
		age = no;
	}
}
