

//Program is: Prog := begin StmtList end
bool Prog(istream& in, int& line)
{
	bool sl = false;
	LexItem tok = Parser::GetNextToken(in, line);
	cout << "in Prog" << endl;
	
	if (tok.GetToken() == BEGIN) {
		sl = StmtList(in, line);
		if( !sl  )
			ParseError(line, "No statements in program");
		if( error_count > 0 )
			return false;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok.GetToken() == END)
		return true;
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else
		return false;
}

// StmtList is a Stmt followed by semicolon followed by a StmtList
 bool StmtList(istream& in, int& line) {
 	cout << "in StmtList" << endl;
	bool status = Stmt(in, line);
	
	if( !status )
		return false;
	LexItem tok = Parser::GetNextToken(in, line);
	
	if( tok == SCOMA ) {
		status = StmtList(in, line);
	}
	else if (tok == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else if (tok == END) {
		Parser::PushBackToken(tok);
		return true;
	}
	else {
		ParseError(line, "Missing semicolon");
		return false;
	}
	return status;
}

//Stmt is either a PrintStmt, IfStmt, or an AssigStmt
bool Stmt(istream& in, int& line) {
	bool status;
	cout << "in Stmt" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case PRINT:
		status = PrintStmt(in, line);
		cout << "status: " << (status? true:false) <<endl;
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
        Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		break;

	case END:
		Parser::PushBackToken(t);
		return true;
	case ERR:
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << t.GetLexeme() << ")" << endl;
		return false;
	case DONE:
		return false;

	default:
		ParseError(line, "Invalid statement");
		return false;
	}

	return status;
}

//PrintStmt:= print ExpreList 
bool PrintStmt(istream& in, int& line) {
	cout << "in PrintStmt" << endl;
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression after print");
		return false;
	}
	
	//Evaluate: print out the list of expressions values

	return ex;
}

//IfStmt:= if (Expr) then Stmt
bool IfStmt(istream& in, int& line) {
	bool ex=false ; 
	LexItem t;
	Value v;
	cout << "in IfStmt" << endl;
	if( (t=Parser::GetNextToken(in, line)) != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	ex = Expr(in, line, v);
	if( !ex ) {
		ParseError(line, "Missing if statement expression");
		return false;
	}

	if((t=Parser::GetNextToken(in, line)) != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	if((t=Parser::GetNextToken(in, line)) != THEN ) {
		
		ParseError(line, "Missing THEN");
		return false;
	}

	bool st = Stmt(in, line);
	if( !st ) {
		ParseError(line, "Missing statement for if");
		return false;
	}
	
	//Evaluate: execute the if statement
	
	return st;
}

//Var:= ident
bool Var(istream& in, int& line, LexItem & tok)
{
	//called only from the AssignStmt function
	string identstr;
	cout << "in Var" << endl;
	LexItem atok = Parser::GetNextToken(in, line);
	
	if (atok == IDENT){
		identstr = atok.GetLexeme();
		if (!(defVar.find(identstr)->second))
			defVar[identstr] = true;
		return true;
	}
	else if(atok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << atok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
	cout << "in AssignStmt" << endl;
	bool varstatus = false, status = false;
	LexItem t;
	Value v;
	
	varstatus = Var( in, line, t);
	cout << "varstatus:" << varstatus << endl;
	
	if (varstatus){
		if ((t=Parser::GetNextToken(in, line)) == EQ){
			status = Expr(in, line, v);
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
			
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator =");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;	
}

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	cout << "in ExprList" << endl;
	Value v;
	status = Expr(in, line, v);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMA) {
		status = ExprList(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value & retVal) {
	bool t1 = Term(in, line, retVal);
	LexItem tok;
	cout << "in Expr" << endl;
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS ) 
	{
		t1 = Term(in, line, retVal);
		if( !t1 ) 
		{
			ParseError(line, "Missing expression after operator");
			return false;
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
		//Evaluate: evaluate the expression for addition or subtraction
	}
	Parser::PushBackToken(tok);
	return true;
}

//Term:= Factor {(*|/) Factor}
bool Term(istream& in, int& line, Value & retVal) {
	cout << "in Term" << endl;
	bool t1 = Factor(in, line, retVal);
	LexItem tok;
	cout << "status of factor1: " << t1<< endl;
	if( !t1 ) {
		return false;
	}
	
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  )
	{
		t1 = Factor(in, line, retVal);
		cout << "status of factor2: " << t1<< endl;
		if( !t1 ) {
			ParseError(line, "Missing expression after operator");
			return false;
		}
		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		//Evaluate: evaluate the expression for multiplication or division
	}
	Parser::PushBackToken(tok);
	return true;
}

//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream& in, int& line, Value & retVal) {
	cout << "in Factor" << endl;
	LexItem tok = Parser::GetNextToken(in, line);
	

	if( tok == IDENT ) {
		//check if the var is defined 
		int val;
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Undefined Variable");
			return false;	
		}
		return true;
	}
	else if( tok == ICONST ) {
		//convert the string of digits to an integer number
		//create a Val object for ICONST and enter into sysmol table
		return true;
	}
	else if( tok == SCONST ) {
		//
		//create a Val object for SICONST and enter into sysmol table
		return true;
	}
	else if( tok == RCONST ) {
		//convert the string of digits to real number
		//create a Val object for RCONST and enter into sysmol table
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after (");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;

		ParseError(line, "Missing ) after expression");
		return false;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	ParseError(line, "Unrecognized input");
	return 0;
}



