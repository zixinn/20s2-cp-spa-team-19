// Dummy ASTs are constructed manually to avoid using Parser methods in DE unit tests.
VarName *varName = new VarName(new sp::Token(sp::Token::TokenType::NAME, "test"), "test");
ProcName *procName = new ProcName(new sp::Token(sp::Token::TokenType::NAME, "test"), "test");
CondExpr *condExpr; // CondExpr is abstract class
PrintStmt *printStmt = new ast::PrintStmt(1, new sp::Token(sp::Token::TokenType::PRINT, "pr"), varName);
ReadStmt *readStmt =  new ast::ReadStmt(1, new sp::Token(sp::Token::TokenType::READ, "r"), varName);
CallStmt *callStmt = new ast::CallStmt(1, new sp::Token(sp::Token::TokenType::CALL, "call"), procName);

std::vector<Stmt*> statements { printStmt, readStmt };
// no stmtLst token to use
StmtLst *stmtLst = new StmtLst(new sp::Token(sp::Token::TokenType::NAME, "test"), statements);
WhileStmt *whileStmt = new ast::WhileStmt(1, new sp::Token(sp::Token::TokenType::WHILE, "w"), condExpr, stmtLst);
WhileStmt *nestedWhileStmt =  new ast::WhileStmt(1, new sp::Token(sp::Token::TokenType::WHILE, "w"), condExpr, stmtLst);
IfStmt *ifStmt =  new ast::IfStmt(1, new sp::Token(sp::Token::TokenType::IF, "i"), condExpr, stmtLst, stmtLst);
IfStmt *nestedIfStmt =  new ast::IfStmt(1, new sp::Token(sp::Token::TokenType::IF, "i"), condExpr, stmtLst, stmtLst);

// The values of the AssignStmts ASTs have to be correct, so dummies cannot be used. They are manually constructed here.
// Set up Assignment ASTs
// scaramouche = tartaglia + pustota * 2 * 0;
VarName *scaramouche = new VarName(new sp::Token(sp::Token::TokenType::NAME, "scaramouche"), "scaramouche");
VarName *tart = new VarName(new sp::Token(sp::Token::TokenType::NAME, "tartaglia"), "tartaglia");
VarName *pustota = new VarName(new sp::Token(sp::Token::TokenType::NAME, "pustota"), "pustota");
ConstVal *constVal2 = new ConstVal(new sp::Token(sp::Token::TokenType::CONST, "2"), 2);
ConstVal *constVal0 = new ConstVal(new sp::Token(sp::Token::TokenType::CONST, "0"), 0);

Expr *nestedExpr = new ast::InfixExpr(new sp::Token(sp::Token::TokenType::TIMES, "*"), pustota, constVal2);
Expr *secondNestedExpr = new ast::InfixExpr(new sp::Token(sp::Token::TokenType::TIMES, "*"), nestedExpr, constVal0);
Expr *expr = new ast::InfixExpr(new sp::Token(sp::Token::TokenType::PLUS, "+"), tart, secondNestedExpr);
AssignStmt *assStmtScaramouche = new ast::AssignStmt(1, new sp::Token(sp::Token::TokenType::ASSIGN, "call"), scaramouche, expr);

// axel2 = semelparity;
VarName *axel2 = new VarName(new sp::Token(sp::Token::TokenType::NAME, "axel2"), "axel2");
VarName *semelparity = new VarName(new sp::Token(sp::Token::TokenType::NAME, "semelparity"), "semelparity");
AssignStmt *assStmtAxel2 = new ast::AssignStmt(1, new sp::Token(sp::Token::TokenType::ASSIGN, "call"), axel2, semelparity);

// slalom = semelparity;
VarName *slalom = new VarName(new sp::Token(sp::Token::TokenType::NAME, "slalom"), "slalom");
AssignStmt *assStmtSlalom = new ast::AssignStmt(1, new sp::Token(sp::Token::TokenType::ASSIGN, "call"), slalom, semelparity);

// quartz = conspiracy;
VarName *quartz = new VarName(new sp::Token(sp::Token::TokenType::NAME, "quartz"), "quartz");
VarName *conspiracy = new VarName(new sp::Token(sp::Token::TokenType::NAME, "conspiracy"), "conspiracy");
AssignStmt *assStmtQuartz = new ast::AssignStmt(1, new sp::Token(sp::Token::TokenType::ASSIGN, "call"), quartz, conspiracy);

// sapphire = 100;
VarName *sapphire = new VarName(new sp::Token(sp::Token::TokenType::NAME, "sapphire"), "sapphire");
ConstVal *constVal100 = new ConstVal(new sp::Token(sp::Token::TokenType::CONST, "100"), 100);
AssignStmt *assStmtSapphire = new ast::AssignStmt(1, new sp::Token(sp::Token::TokenType::ASSIGN, "call"), sapphire, constVal100);

// axel = 420;
VarName *axel = new VarName(new sp::Token(sp::Token::TokenType::NAME, "axel"), "axel");
ConstVal *constVal420 = new ConstVal(new sp::Token(sp::Token::TokenType::CONST, "420"), 420);
AssignStmt *assStmtAxel = new ast::AssignStmt(1, new sp::Token(sp::Token::TokenType::ASSIGN, "call"), axel, constVal420);

// eternal = senescence + sequestration * 2 * 0;
VarName *eternal = new VarName(new sp::Token(sp::Token::TokenType::NAME, "eternal"), "eternal");
VarName *senescence = new VarName(new sp::Token(sp::Token::TokenType::NAME, "senescence"), "senescence");
VarName *sequestration = new VarName(new sp::Token(sp::Token::TokenType::NAME, "sequestration"), "sequestration");

Expr *nestedExprE = new ast::InfixExpr(new sp::Token(sp::Token::TokenType::TIMES, "*"), sequestration, constVal2);
Expr *secondNestedExprE = new ast::InfixExpr(new sp::Token(sp::Token::TokenType::TIMES, "*"), nestedExprE, constVal0);
Expr *exprE = new ast::InfixExpr(new sp::Token(sp::Token::TokenType::PLUS, "+"), senescence, secondNestedExprE);
AssignStmt *assStmtEternal = new ast::AssignStmt(1, new sp::Token(sp::Token::TokenType::ASSIGN, "call"), eternal, exprE);