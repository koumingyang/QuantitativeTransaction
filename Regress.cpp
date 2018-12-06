#include "Regress.h"

vec regress(vec x, mat y, int times = 1)
{
    int n = times;
    int m = x.n_elem;

    mat a = zeros<mat>(n+1, n+1);
    vec b = zeros<vec>(n+1);
    vec c = zeros<vec>(n+1);
    vec d = zeros<vec>(n+n+1);

    int i, j, k, p;
    double now, tmp, sum, tt;
    for (i = 0; i <= n+n; i++)
        for (j = 0; j < m; j++)
        {
            for (k = 1, now = 1.0; k <= i; k++) now *= x(j);
            d(i) += now;
            if (i <= n) b(i) += now * y(j, 1);
        }
    
    for (i = 0; i <= n; i++)
        for (j = 0; j <= n; j++)
            a(i, j) = d(i+j);
    
	for(i = 0; i < n; i++)
	{
		tmp = 0;
		for(j = i; j <= n; j++)
			if (fabs(a(i, j)) > tmp)
				tmp = fabs(a(i, j)), p = j;
		if(p != i)
		{
            tt = b(p); b(p) = b(i); b(i) = tt;
			for(j = i; j <= n; j++)
                {tt = a(i, j); a(i, j) = a(p, j); a(p, j) = tt;}
		}
		for(k = i+1; k <= n; k++)
		{
			tmp = a(k, i) / a(i, i);
			b(k) -= tmp * b(i);
			for(j = i; j <= n; j++)
				a(k, j) -= tmp * a(i, j);
		}
	}
    c(n) = b(n) / a(n, n);
	for(i = n-1; i >= 0; i--)
	{
		sum = 0;
		for(j = i+1; j <= n; j++)
            sum += a(i, j) * c(j);
        c(i) = (b(i) - sum) / a(i, i);
	}

    return c;
}