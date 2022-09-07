let count = 0;
let array=[]
function countUp() {
    let counter = document.getElementById('counter');
    count = count + 1;
    counter.innerText = count;
}
function countDown() {
    let counter = document.getElementById('counter');
    count = count - 1;
    counter.innerText = count;
} 
function getInput() {
    let input = document.getElementById("todo-input")
    let todo = document.createElement("div");
    array.push(input.value);
    render()
}
function render() {
    let todolist = document.getElementById("todo-list");
    todolist.innerHTML=''
    array.forEach(function (value) {
        let element = document.createElement("div");
        element.innerText = value;
        todolist.appendChild(element);
    });
}