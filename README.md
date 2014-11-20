<b>BigInteger:</b><br>
&emsp;&emsp;Реализовать класс BigInteger, который (звездочками отмечены необязательные пункты):<br>
&emsp;&emsp;1) Имеет конcтруктор по умолчанию, создающий объект равный нулю<br>
&emsp;&emsp;2) Имеет конструктор от int<br>
&emsp;&emsp;3*) Имеет конструктор от std::string<br>
&emsp;&emsp;4*) Может быть приведен к std::string<br>
&emsp;&emsp;5) Имеет конструктор копирования<br>
&emsp;&emsp;6) Имеет оператор присваивания<br>
&emsp;&emsp;7) Поддерживает операторы +, -, *<br>
&emsp;&emsp;8) Поддерживает операторы +=, -=, *=<br>
&emsp;&emsp;9*) Поддерживает операторы /, %, /=, %=<br>
&emsp;&emsp;10*) Имеет move-конструктор<br>
&emsp;&emsp;11) Может быть прочитан из потока (std::istream)<br>
&emsp;&emsp;12) Может быть выведен в поток (std::ostream)<br>
&emsp;&emsp;13) Конструктор от std::string и оператор чтения из потока бросают адекватные исключения при ошибках<br>
<hr>
<b>Покер</b><br>
Задание - написать приложение для игры в покер нескольких игроков на одном компьютере. Интерфейс произвольный, вполне достаточно консольного вида вопрос-ответ. Правила любые из реально существующих<br>

&emsp;&emsp;Следует придерживаться парадигм ООП<br>
&emsp;&emsp;Карты бывают открытыми и закрытыми, очевидно, что открытая карта может стать закрытой, но никогда наоборот (при этом, как подсказывает здравый смысл, карта может быть открыта только для одного игрока)<br>
&emsp;&emsp;Класс игрок-человек должен быть реализацией некоторого абстрактного класса Игрок, для которого должна быть возможность подставить реализацию компьютерного игрока.<br>
&emsp;&emsp;Для компьютерного игрока не должно быть возможности жульничать, причем создание новых, уничтожение существующих или изменение карт должны приводить к ошибкам при компиляции<br>
Бонусы:<br>
&emsp;&emsp;Корректная обработка любого некорректного ввода пользователя<br>
&emsp;&emsp;Покрытие кода тестами с использованием любого unittest фреймворка<br>
&emsp;&emsp;Универсальная реализация (конкретные правила игры - реализация некоторого абстрактного класса)<br>
<hr>
<b>DFS</b><br>
Реализовать представление графа с реализацией dfs:<br>
<br>
template<typename NodePayload><br>
class Graph {<br>
public:<br>
&emsp;&emsp;typedef unsigned NodeHandle;<br>
&emsp;&emsp;typedef std::pair<NodeHandle, NodeHandle> EdgeHandle;<br>
&emsp;&emsp;typedef std::function<void(NodeHandle const &)> NodeVisitor;<br>
&emsp;&emsp;typedef std::function<void(EdgeHandle const &)> EdgeVisitor;<br>
public:<br>
&emsp;&emsp;Graph();<br>
&emsp;&emsp;~Graph();<br>
&emsp;&emsp;void loadFromFile(std::string const & filename);<br>
&emsp;&emsp;void saveToFile(std::string const & filename);<br>
&emsp;&emsp;NodeHandle addNode();<br>
&emsp;&emsp;void addEdge(NodeHandle const & a, NodeHandle const & b);<br>
&emsp;&emsp;void forEachNode(std::function<void(NodeHandle const &)> const & visitor) const;<br>
&emsp;&emsp;size_t getNodesCount() const;<br>
&emsp;&emsp;void forEachEdge(NodeHandle const & source, EdgeVisitor const & visitor);<br>
&emsp;&emsp;NodeHandle move(NodeHandle const & origin, EdgeHandle const & edge);<br>
&emsp;&emsp;NodePayload & operator[](NodeHandle const & node);<br>
&emsp;&emsp;void dfs(NodeVisitor const & startNode, NodeVisitor const & endNode, NodeVisitor const & discoverNode);<br>
};<br>
<br>
Вы можете заменить std::function на абстрактные классы(для dfs все три вызова - методы одного класса с такими же именами, как параметры предложенного метода, у остальных - оператор () с соотвествующим аргументом).<br>

Решение не должно содержать рекрсивных вызовов.<br>

Задание оценивается из 10+5 баллов. Бонусные баллы будут даваться за скорость работы приложения.<br>
