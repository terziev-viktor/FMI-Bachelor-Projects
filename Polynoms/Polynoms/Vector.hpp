#pragma once

namespace polynoms
{
	namespace tools
	{
		// Vector of elements of type T
		// T should have a copy constructor
		template<typename T>
		class Vector
		{
		public :
			Vector();
			~Vector();

			static const unsigned int INIT_SIZE = 15;

			void setAt(unsigned int at, const T & item);
			void print();

			// overrride
			void add(const T & item);
			const T & getAt(unsigned int index) const;
			void removeAt(unsigned int index);
			const bool isEmplty() const;
			void clear();
			const unsigned int count() const;
		private:
			unsigned int index;
			unsigned int size;
			T * buffer;

			void expand();
		};
		
		template<typename T>
		inline Vector<T>::Vector()
		{
			this->size = Vector::INIT_SIZE;
			this->index = 0;
			this->buffer = new T[this->size];
		}

		template<typename T>
		inline Vector<T>::~Vector()
		{
			if (this->buffer)
			{
				delete[] this->buffer;
			}
		}

		template<typename T>
		inline void Vector<T>::setAt(unsigned int at, const T & item)
		{
			if (at >= 0 && at <= this->index)
			{
				this->buffer[at] = item;
			}
		}

		template<typename T>
		inline void Vector<T>::print()
		{
			for (unsigned int i = 0; i < this->index; i++)
			{
				cout << this->buffer[i] << " ";
			}
		}

		template<typename T>
		inline void Vector<T>::add(const T & item)
		{
			if (this->index == this->size)
			{
				this->expand();
			}
			this->buffer[this->index] = item;
			++this->index;
		}

		template<typename T>
		inline const T & Vector<T>::getAt(unsigned int index) const
		{
			// todo: check for valid argument

			return this->buffer[this->index];
		}

		template<typename T>
		inline void Vector<T>::removeAt(unsigned int index)
		{
			if (index >= 0 && index <= this->index)
			{
				for (unsigned int i = index; i < this->index; i++)
				{
					this->buffer[i] = this->buffer[i + 1];
				}
				--this->index;
			}
		}

		template<typename T>
		inline const bool Vector<T>::isEmplty() const
		{
			return this->index == 0;
		}

		template<typename T>
		inline void Vector<T>::clear()
		{
			this->index = 0;
		}

		template<typename T>
		inline const unsigned int Vector<T>::count() const
		{
			return this->index;
		}

		template<typename T>
		inline void Vector<T>::expand()
		{
			this->size *= 2;
			T * biggerBuffer = new T[this->size];
			for (unsigned int i = 0; i < this->index; i++)
			{
				biggerBuffer[i] = this->buffer[i];
			}
			delete[] this->buffer;
			this->buffer = biggerBuffer;
		}

	}
}