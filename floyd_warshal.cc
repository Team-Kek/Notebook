void bellman_ford(int **d, int n)
{
  int i,j,k;
  for (k = 0; k < n; k++)
    for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
        if (d[i][j] > d[i][k] + d[k][j])
          d[i][j] = d[i][k] + d[k][j];
}
