//Neighborhoods
//Patrick Cunniff


void twoOptKnapsack(knapsack &k, int t, clock_t start)
//switches (in/out) of each item in knapsack, return by reference best knapsack
{
	double time=0;
	bool cont =true;
	knapsack best=k;
	clock_t end;
	while(cont && (time<t))
	{
		for(int i=0; i<k.getNumObjects(); i++)
		{
			if(k.isSelected(i))
				k.unSelect(i);
			else k.select(i);

			if((best.getCurrentValue())<(k.getCurrentValue()) && (k.getCurrentCost())<(k.getCostBound()))
				best=k;

			if(k.isSelected(i))
				k.unSelect(i);
			else k.select(i);
		}

		if((best.getCurrentValue())==(k.getCurrentValue()))
			cont=false;

		k=best;

		end=clock();
		time=((double)(end-start))/CLOCKS_PER_SEC;
	}
}

void threeOptKnapsack(knapsack &k, int t, clock_t start)
//switches (in/out) of each item 2 at a time in knapsack,
//return by reference best knapsack
{
	double time=0;
	bool cont =true;
	knapsack best=k;
	clock_t end;

	while(cont && (time<t))
	{
		for(int i=0; i<k.getNumObjects()-1; i++)
		{
			for(int j=i+1; j<k.getNumObjects(); j++)
			{
				if(k.isSelected(i))
					k.unSelect(i);
				else k.select(i);

				if(k.isSelected(j))
					k.unSelect(j);
				else k.select(j);

				if((best.getCurrentValue())< (k.getCurrentValue()) && (k.getCurrentCost())<(k.getCostBound()))
					best=k;

				if(k.isSelected(i))
					k.unSelect(i);
				else k.select(i);

				if(k.isSelected(j))
					k.unSelect(j);
				else k.select(j);
			}
		}

		if((best.getCurrentValue())==(k.getCurrentValue()))
			cont=false;
		k=best;

		end=clock();
		time=((double)(end-start))/CLOCKS_PER_SEC;
	}
}

int twoOptColoring(graph &g,int numColors, int t, clock_t start)
//switch color of each node to every color
//return by reference best graph
{
	double time=0;
	bool cont =true;
	int conflicts=99999,color,currConflicts=0;
	graph best=g;
	clock_t end;
	while(cont && (time<t))
	{
		for(int i=0; i<g.numNodes(); i++)
		{
			color=g.getColor(i);
			for(int j=0; j<numColors; j++)
			{
				g.setColor(i,j);

				for(int p=0;p<g.numNodes();p++)
				{
					for(int q=0;q<g.numNodes();q++)
					{
						if(g.isEdge(q,p)||g.isEdge(p,q))
							if(g.getColor(p)==g.getColor(q))
								currConflicts++;
					}
				}
				currConflicts/=2;

				if(currConflicts<conflicts)
				{
					conflicts=currConflicts;
					best=g;
				}
				currConflicts=0;
			}
			g.setColor(i,color);
		}

		if(currConflicts==conflicts)
			cont=false;
		g=best;
		end=clock();
		time=((double)(end-start))/CLOCKS_PER_SEC;
	}
	return conflicts;
}

int threeOptColoring(graph &g,int numColors, int t, clock_t start)
//switch color of each node to every color, 2 at a time
//return by reference best graph
{
	double time=0;
	bool cont =true;
	int conflicts=99999, color1, color2, currConflicts=0;
	graph best=g;
	clock_t end;
	while(cont && (time<t))
	{
		for(int i=0; i<g.numNodes()-1; i++)
		{
			color1=g.getColor(i);
			color2=g.getColor(i+1);
			for(int j=0; j<numColors; j++)
			{
				g.setColor(i,j);
				for(int k=0; k<numColors; k++)
				{
					g.setColor(i+1,k);
					for(int p=0;p<g.numNodes();p++)
					{
						for(int q=0;q<g.numNodes();q++)
						{
							if(g.isEdge(q,p)||g.isEdge(p,q))
								if(g.getColor(p)==g.getColor(q))
									currConflicts++;
						}
					}
					currConflicts/=2;

					if(currConflicts<conflicts)
					{
						conflicts=currConflicts;
						best=g;
					}
					currConflicts=0;
				}
			}
			g.setColor(i,color1);
			g.setColor(i+1,color2);
		}

		if(currConflicts==conflicts)
			cont=false;
		g=best;
		end=clock();
		time=((double)(end-start))/CLOCKS_PER_SEC;
	}
	return conflicts;
}