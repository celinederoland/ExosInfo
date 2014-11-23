package chaineListe;

public class Element  {

	char elem;
	Element suiv;
	
	public Element (char elem, Element suiv)
	{
		this.elem = elem;
		this.suiv = suiv;
	}
	
	public Element (char elem)
	{
		this.elem = elem;
		this.suiv = null;
	}
	
	public char getElement()
	{
		return elem;
	}
	
	public void setElem(char elem)
	{
		this.elem = elem;
	}
	
	public Element getSuivant()
	{
		return suiv;
	}
	
	public void setSuivant (Element suiv)
	{
		this.suiv = suiv;
	}
}
