#ifdef DEBUG
traceflag = tracetmp;
if (debuglevel & 256 || ((debuglevel & 1024) && traceflag &&
			 (!traceply || ply <= traceply)))
  {
    int i;
    algbr (node->f, node->t, node->flags);
    for (i = 1; i < ply; i++)
      printf ("   |");
    printf ("%s S%d d%d p%d %d %x n->s%d s%d a%d b%d best%d x%d\n", 
	mvstr[0], Sdepth, depth, ply, pnt,node->flags, node->score, score, alpha, beta, best, xxxtmp);
  }

#endif
