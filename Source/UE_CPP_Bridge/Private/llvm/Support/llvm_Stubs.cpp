
#include <string>

namespace llvm {
// DenseMap сам весь в .h файлах. Но тянет зависимости из llvm/Support.
// Причем в основном репорты об ошибках. Обойдемся. Заглушки вот сделаем.

// Заглушка для Twine::str() — линкер просит реализацию преобразования в std::string
// Мы возвращаем пустую строку, так как в UE этот код не должен сработать
//class Twine;
std::string Twine::str() const { return ""; }

// Заглушка для фатальных ошибок — перенаправляем в лог Unreal
void report_fatal_error(class Twine const& Message, bool bGenCrashDiag) {
	check(0);
	//UE_LOG(LogTemp, Fatal, TEXT("LLVM Fatal Error encountered!"));
}

// Если линкер попросит саму структуру Twine или потоки:
class raw_ostream;
void report_fatal_error(char const* Msg, bool bFatal) {
	//UE_LOG(LogTemp, Fatal, TEXT("LLVM Fatal: %s"), *FString(Msg));
}
void report_bad_alloc_error(const char* Msg, bool bFatal) {
	report_fatal_error(Msg, bFatal);
}
} // namespace llvm

