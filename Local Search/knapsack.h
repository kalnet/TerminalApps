//knapsack class w/ added functions from project 4

class knapsack
{
public:
	knapsack();
	knapsack(ifstream &fin);
	knapsack(const knapsack &k);
	knapsack &operator=(const knapsack &k);

	int getCost(int) const;
	int getValue(int) const;
	int getIndex(int) const;

	int getCurrentValue() const;
	int getCurrentCost() const;

	bool isSelected(int) const;

	void setCost(int, int);
	void setValue(int, int);
	void setIndex(int, int);

	void select(int);
	void unSelect(int);
	void unSelectAll();

	int getNumObjects() const;
	int getCostBound() const;
	void printSolution(const vector<bool> &solution) const; 
	void printSolution();
	void sortObjects();

	void setNum(int);
	int getNum() const;

	void outFile(const knapsack&, string);

private:
	int numObjects;
	int costBound;
	vector<int> value;
	vector<int> cost;
	vector<int> index;
	vector<bool> selected;
	int num;
	int currentValue;
	int currentCost;
};

knapsack::knapsack()
// Create a blank knapsack instance
{
	numObjects = 0;
	costBound = 0;

	value.resize(0);
	cost.resize(0);
	index.resize(0);
	selected.resize(0);

	setNum(0);
	currentValue = 0;
	currentCost = 0;
}

knapsack::knapsack(ifstream &fin)
// Construct a new knapsack instance using the data in fin.  Mark all
// objects as unselected.
{
	int n, b, j, v, c;

	fin >> n;  // read the number of objects
	fin >> b;  // read the cost bound

	numObjects = n;
	costBound = b;

	value.resize(n);
	cost.resize(n);
	index.resize(n);
	selected.resize(n);

	for (int i = 0; i < n; i++)
	{
		fin >> j >> v >> c;
		setValue(j,v);
		setCost(j,c);
		setIndex(i,i);
		unSelect(j);
	}

	setNum(0);
	currentValue = 0;
	currentCost = 0;
}

knapsack::knapsack(const knapsack &k)
// Knapsack copy constructor
{
	numObjects = k.getNumObjects();
	costBound = k.getCostBound();

	value.resize(numObjects);
	cost.resize(numObjects);
	index.resize(numObjects);
	selected.resize(numObjects);

	for (int i = 0; i < numObjects; i++)
	{
		setValue(i,k.getValue(i));
		setCost(i,k.getCost(i));
		setIndex(i,k.getIndex(i));

		if (k.isSelected(i))
			select(i);
		else
			unSelect(i);
	}

	setNum(k.getNum());
	currentValue = k.getCurrentValue();
	currentCost = k.getCurrentCost();
}

knapsack &knapsack::operator=(const knapsack &k)
// Knapsack assignment operator
{
	numObjects = k.getNumObjects();
	costBound = k.getCostBound();

	value.resize(numObjects);
	cost.resize(numObjects);
	index.resize(numObjects);
	selected.resize(numObjects);

	for (int i = 0; i < numObjects; i++)
	{
		setValue(i,k.getValue(i));
		setCost(i,k.getCost(i));
		setIndex(i,k.getIndex(i));

		if (k.isSelected(i))
			select(i);
		else
			unSelect(i);
	}

	setNum(k.getNum());
	currentValue = k.getCurrentValue();
	currentCost = k.getCurrentCost();

	return *this;
}

int knapsack::getNumObjects() const
{
	return numObjects;
}

int knapsack::getCostBound() const
{
	return costBound;
}

int knapsack::getValue(int i) const
// Return the value of the ith object.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::getValue");

	return value[i];
}

int knapsack::getCost(int i) const
// Return the cost of the ith object.
{
	if (i < 0 || i >= getNumObjects())
	{
		throw rangeError("Bad value in knapsack::getCost");
	}

	return cost[i];
}

int knapsack::getCurrentValue() const
// Return the total value of the selected objects
{
	return currentValue;
}

int knapsack::getCurrentCost() const
// Return the total cost of the selected objects
{
	return currentCost;
}

int knapsack::getIndex(int i) const
// Return the index of the ith object.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::getIndex");

	return index[i];
}

bool knapsack::isSelected(int i) const
// Returns true if object i has been selected, and false
// otherwise.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::isSelected");

	return selected[i];
}

void knapsack::setValue(int i, int n) 
// Set the ith object's value to n.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::setValue");

	value[i] = n;
}

void knapsack::setCost(int i, int n) 
// Set the ith object's cost to n.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::getCost");

	cost[i] = n;
}

void knapsack::setIndex(int i, int n) 
// Set the ith object's index to n.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::setIndex");

	index[i] = n;
}

void knapsack::select(int i)
// Select the ith object and update current value and cost
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::select");

	if (!selected[i])
	{
		currentValue = currentValue + getValue(i);
		currentCost = currentCost + getCost(i);
	}
	selected[i] = true;
}

void knapsack::unSelect(int i)
// Unselect the ith object and update current value and cost
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::unSelect");

	if (selected[i])
	{
		currentValue = currentValue - getValue(i);
		currentCost = currentCost - getCost(i);
	}
	selected[i] = false;
}

void knapsack::unSelectAll()
// Unselect all objects
{
	for (int i = 0; i < getNumObjects(); i++)
		unSelect(i);
}


void knapsack::setNum(int n)
// Update the number of objects that have been considered.
{
	num = n;
}

int knapsack::getNum() const
// Return the number of objects that have been considered.
{
	return num;
}

ostream &operator<<(ostream &ostr, const knapsack &k)
// Print all information about the knapsack.
{
	cout << "------------------------------------------------" << endl;
	cout << "Num objects: " << k.getNumObjects() << " Cost bound: " << k.getCostBound() << endl;

	int totalValue = 0;
	int totalCost = 0;

	cout << "Total value: " << k.getCurrentValue() << endl;
	cout << "Total cost: " << k.getCurrentCost() << endl << endl;

	for (int i = 0; i < k.getNumObjects(); i++)
		if (k.isSelected(i))
			cout << k.getIndex(i) << "  " << k.getValue(i) << " " << k.getCost(i) << endl;

	cout << endl;

	return ostr;
}

void knapsack::printSolution(const vector<bool> &solution) const
// Prints out the solution.
{
	cout << "------------------------------------------------" << endl;

	int totalValue = 0;
	int totalCost = 0;

	// Add up values for objects in the solution

	for (int i = 0; i < getNumObjects(); i++)
	{
		if (solution[i])
		{
			totalValue += getValue(i);
			totalCost += getCost(i);
		}
	}

	cout << "Total value: " << totalValue << endl;
	cout << "Total cost: " << totalCost << endl << endl;

	// Print out objects in the solution
	for (int i = 0; i < getNumObjects(); i++)
		if (solution[i])
			cout << getIndex(i) << "  " << getValue(i) << " " << getCost(i) << endl;

	cout << endl;
}

ostream &operator<<(ostream &ostr, vector<bool> v)
// Overloaded output operator for vectors.
{
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << endl;

	return ostr;
}

void knapsack::sortObjects()
// Sort the objects by decreasing ratio of value/cost using insertion
// sort.
{
	int value, cost, index;
	int j;

	cout << "Sorting objects" << endl;

	for (int i = 1; i < getNumObjects(); i++)
	{
		value = getValue(i);
		cost = getCost(i);
		index = getIndex(i);

		j = i-1;
		while (j >= 0 && (float) getValue(j) / getCost(j) < (float) value/cost)
		{
			setValue(j+1,getValue(j));
			setCost(j+1,getCost(j));
			setIndex(j+1,getIndex(j));
			j = j-1;
		}
		setValue(j+1,value);
		setCost(j+1,cost);
		setIndex(j+1,index);
	}
}

void knapsack::outFile(const knapsack &B,string whichOne)
{
	char x, buffer [5];
	string ofileName;
	ofstream file;

	snprintf(buffer, 10, "%d", getNumObjects());
	//itoa (getNumObjects(),buffer,10);
	ofileName = "knapsack";
	ofileName += buffer ;
	ofileName += ".output";
	cout << ofileName << endl;

	file.open (ofileName.c_str());
	streambuf* sbuf = cout.rdbuf();
	cout.rdbuf(file.rdbuf());
	cout<<whichOne<<endl;
	printSolution(B.selected);
	cout.rdbuf(sbuf);
	file.close();
}