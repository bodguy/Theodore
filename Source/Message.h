#ifndef Message_h
#define Message_h

namespace Quark {
	enum MessageType {
		OnCollisionEnter,
		OnCollisionExit,
		OnCollisionStay,
		TheNumberOfMessage
	};

	enum class MessageResult {
		False,
		True,
		Ignored,
		Error
	};

	class Message {
	public:
		Message();
		explicit Message(MessageType type);
		Message(MessageType type, void* data);

		// copy, operator
		Message(const Message& rhs);
		Message& operator =(const Message& rhs);

		MessageType GetType() const;
		void* GetData() const;

	private:
		MessageType mType;
		void* mData;
	};
}
#endif /* Message_h */
