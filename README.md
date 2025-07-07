# Chronic
Библиотека, обертывающая сырые сокеты WinSock2. <br/>
Стандарт языка: 20+                             <br/>
OS: Win (only)                                  <br/>

# Подключение
Добавьте папку chronic в компиляцию.

# Примеры

### Сообщение Host -> Client.
Код клиента:
```cpp
#include <chronic.h>
int main() {
	chronic::Host server{ "IP",PORT };
	const chronic::Connected* client = server.waitClient();
	std::cout << client->receive();
	system("pause");
	return EXIT_SUCCESS;
}
```
Код Хоста:
```cpp
#include <chronic.h>
int main() {
  chronic::Client user{ IP,PORT,"MAIN" };
  if (!user.connect()) {
  	system("pause");
  	return EXIT_FAILURE;
  }
  user.send("Here a test message for you!");
  system("pause");
  return EXIT_SUCCESS;
}
```

### Управление debug.h
(Почти) каждое действие записывается и выводиться.                   <br/>
Если вы используете консоль для работы, то это может помешать.       <br/>
Чтобы заглушить вывод, нужно вызвать функцию:                        
```cpp
chronic::debug::setMode(chronic::debug::MODE::SILENT); // or just chronic::debug::setMode(0);
```

Каждая запись имеет свой тип, поэтому при получении всех записей нужно указать тип.   <br/>
Для получения списка всех записей используется:
```cpp
chronic::debug::getEntries(chronic::debug::TYPE)
```
