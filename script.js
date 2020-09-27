// const identifier in JS means that the variable identifier
// cannot be reassigned, but the content of this object changable
const listContainer = document.querySelector('[data-list-container]')
const newListForm = document.querySelector('[data-new-list-form]');
const newListInput = document.querySelector('[data-new-list-input]');
const deleteListBtn = document.querySelector('[data-delete-list-btn]');
const taskDisplay = document.querySelector('[data-task-display]');
const listTitle = document.querySelector('[data-list-title]');
const taskCount = document.querySelector('[data-task-count]');
const taskContainer = document.querySelector('[data-task-container]');
const taskTemplate = document.getElementById('task-template');
const newTaskForm = document.querySelector('[data-new-task-form]');
const newTaskInput = document.querySelector('[data-new-task-input]');
const clearTaskBtn = document.querySelector('[data-clear-task-btn]');

// save to local storage and query with this key
const LOCAL_STORAGE_KEY = 'Todo.list';
const LOCAL_STORAGE_LIST_KEY = 'Todo.selectedListID';

// list would take the form of
// {
//     id: 1,
//     name: 'test',
//     tasks: []
// }
let lists = JSON.parse(localStorage.getItem(LOCAL_STORAGE_KEY)) || [];
let selectedListID = JSON.parse(localStorage.getItem(LOCAL_STORAGE_LIST_KEY));

listContainer.addEventListener('click',
    event => {
        if (event.target.tagName.toLowerCase() === 'li') {
            selectedListID = event.target.dataset.listID;
            save();
            render();
        }
    }
)

taskContainer.addEventListener('click',
    event => {
        if (event.target.tagName.toLowerCase() === 'input') {
            const selectedList = lists.find(list => list.id === selectedListID);
            const selectedTask = selectedList.tasks.find(task => task.id === event.target.id);
            selectedTask.complete = event.target.checked;
            save();
            renderTaskCount(selectedList.tasks);
       }
    }
);

newListForm.addEventListener('submit',
    event => {
        event.preventDefault(); // don't refresh page when form is submitted
        const newListName = newListInput.value;
        if (newListName == null || newListName === '') return;
        lists.push(creatList(newListName));
        newListInput.value = null;
        save();
        render();
    }
);

newTaskForm.addEventListener('submit',
    event => {
        event.preventDefault();
        const newTaskName = newTaskInput.value;
        if (newTaskName == null || newTaskName === '') return;
        const selectedList = lists.find(list => list.id === selectedListID);
        selectedList.tasks.push(creatTask(newTaskName));
        newTaskInput.value = null;
        save();
        renderTasks();
    }
);

deleteListBtn.addEventListener('click',
    event => {
        lists = lists.filter(list => list.id !== selectedListID);
        selectedListID = null;
        save();
        render();
    }
)

clearTaskBtn.addEventListener('click',
    event => {
        const selectedList = lists.find(list => list.id === selectedListID);
        selectedList.tasks = selectedList.tasks.filter(task => !task.complete);
        save();
        renderTasks();
    }
);

function save() {
    localStorage.setItem(LOCAL_STORAGE_KEY, JSON.stringify(lists));
    localStorage.setItem(LOCAL_STORAGE_LIST_KEY, JSON.stringify(selectedListID));
}

function creatList(name) {
    // task take the form of
    // {
    //     id: 1,
    //     name: 'test',
    //     complete: false
    // }
    return {id: Date.now().toString(), name: name, tasks: []};
}

function creatTask(name) {
    return {id: Date.now().toString(), name: name, complete: false};
}

function clearElements(target) {
    while (target.lastChild) {
        target.removeChild(target.lastChild);
    }
}

function creatListElement(list) {
    const listElement = document.createElement('li');
    listElement.classList.add("list-name");
    listElement.dataset.listID = list.id;
    listElement.innerText = list.name;
    return listElement;
}

function renderList() {
    clearElements(listContainer);
    lists.forEach(
        list => {
            const listElement = creatListElement(list);
            listContainer.appendChild(listElement);
            if (list.id === selectedListID) {
                listElement.classList.add('active-list');
            }
        }
    );
}

function creatTaskElement(task) {
    // create with template
    const taskElement = document.importNode(taskTemplate.content, true);
    const checkBox = taskElement.querySelector('input');
    const label = taskElement.querySelector('label');
    checkBox.id = task.id;
    checkBox.checked = task.complete;
    label.htmlFor = task.id; // need to use htmlFor for the 'for'
    label.append(task.name);
    return taskElement
}

function renderTaskCount(tasks) {
    const count = tasks.filter(task => !task.complete).length;
    const taskString = count < 2 ? 'task' : 'tasks';
    taskCount.innerText = `${count} ${taskString} remaining`;
}

function renderTasks() {
    clearElements(taskContainer);
    const selectedList = lists.find(list => list.id === selectedListID);
    listTitle.innerText = selectedList.name;
    selectedList.tasks.forEach(
        task => {
            const taskElement = creatTaskElement(task);
            taskContainer.appendChild(taskElement);
        }
    );
    renderTaskCount(selectedList.tasks);
}

function render() {
    renderList();
    if (selectedListID == null) {
        taskDisplay.style.display = 'none';
    } else {
        taskDisplay.style.display = '';
        renderTasks();
    }
}

render();