#include "utils.h"

namespace TestUnsafeRef
{

#define TESTNAME "TestUnsafeRef"

static const char *script1 =
"void Test()                            \n"
"{                                      \n"
"   int[] array = {0};                  \n"
"   TestRefInt(array[0]);               \n"
"   Assert(array[0] == 23);             \n"
"   int a = 0;                          \n"
"   TestRefInt(a);                      \n"
"   Assert(a == 23);                    \n"
"   string[] sa = {\"\"};               \n"
"   TestRefString(sa[0]);               \n"
"   Assert(sa[0] == \"ref\");           \n"
"   string s = \"\";                    \n"
"   TestRefString(s);                   \n"
"   Assert(s == \"ref\");               \n"
"}                                      \n"
"void TestRefInt(int &ref)              \n"
"{                                      \n"
"   ref = 23;                           \n"
"}                                      \n"
"void TestRefString(string &ref)        \n"
"{                                      \n"
"   ref = \"ref\";                      \n"
"}                                      \n";

bool Test()
{
	if( !strstr(asGetLibraryOptions(), "AS_ALLOW_UNSAFE_REFERENCES") )
	{
		printf("%s: This test is only valid with AS_ALLOW_UNSAFE_REFERENCES\n", TESTNAME);
		return false;
	}

	bool fail = false;
	int r;

	COutStream out;

 	asIScriptEngine *engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	engine->SetMessageCallback(asMETHOD(COutStream,Callback), &out, asCALL_THISCALL);

	RegisterScriptString(engine);

	r = engine->RegisterGlobalFunction("void Assert(bool)", asFUNCTION(Assert), asCALL_GENERIC); assert( r >= 0 );

	engine->AddScriptSection(0, TESTNAME, script1, strlen(script1), 0);
	r = engine->Build(0);
	if( r < 0 )
	{
		fail = true;
		printf("%s: Failed to compile the script\n", TESTNAME);
	}
	asIScriptContext *ctx = 0;
	r = engine->ExecuteString(0, "Test()", &ctx);
	if( r != asEXECUTION_FINISHED )
	{
		fail = true;
		printf("%s: Execution failed: %d\n", TESTNAME, r);
	}

	if( ctx ) ctx->Release();


	engine->Release();

	// Success
	return fail;
}

} // namespace
